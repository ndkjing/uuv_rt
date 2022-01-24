/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#include <lora_24.h>
#include <manager.h>


#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       2048
#define THREAD_TIMESLICE        5

static rt_thread_t thread_lora_rec = RT_NULL;
//static rt_thread_t thread_lora_send = RT_NULL;

#define LORA_UART_NAME       "uart2"    /* 串口设备名称 */
static rt_device_t lora_serial;                /* 串口设备句柄 */
struct serial_configure lora_config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */

struct rt_semaphore lora_sem;  // 信号量


rt_err_t rx_callback(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&lora_sem);
    return RT_EOK;
}

void init_lora()
{
    /* step1：查找串口设备 */
    lora_serial = rt_device_find(LORA_UART_NAME);
    /* step2：修改串口配置参数 */
    lora_config.baud_rate = BAUD_RATE_115200;        //修改波特率为 9600
    lora_config.data_bits = DATA_BITS_8;           //数据位 8
    lora_config.stop_bits = STOP_BITS_1;           //停止位 1
    lora_config.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    lora_config.parity    = PARITY_NONE;           //无奇偶校验位

    /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(lora_serial, RT_DEVICE_CTRL_CONFIG, &lora_config);

    /* step4：打开串口设备。以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(lora_serial, RT_DEVICE_FLAG_INT_RX);
    /* 初始化信号量 */
    rt_sem_init(&lora_sem, "lora_sem", 0, RT_IPC_FLAG_FIFO);
    rt_device_set_rx_indicate(lora_serial, rx_callback);
}

/* 线程 的入口函数 */
//static void lora_send(void *parameter)
//{
//    char str[] = "RT-ok!\r\n";
//    while (1)
//    {
//        if (lng>100.0) {
//            rt_device_write(lora_serial, 0, str, (sizeof(str) - 1));  //向指定串口发送数据
//        }
//        rt_thread_mdelay(300);
//    }
//}

// 往串口发送指定数据
void send_data(char* buf,int len)
{
    rt_device_write(lora_serial, 0, buf, len);  //向指定串口发送数据
}

static void lora_rec(void *parameter)
{
    char lora_char;//接收字节
    char step_buffer[32]={0};//数据
    int step_buffer_index=0;//索引
    char pwm_buffer[32]={0};//数据
    int pwm_buffer_index=0;//索引
    char line_buffer[32]={0};//数据
    int line_buffer_index=0;//索引
    char step1_s[5]={0};
    char step2_s[5]={0};
    char pwm1_s[5]={0};
    char pwm2_s[5]={0};
    char line_s[5]={0};
    while (1)
    {
        while(rt_device_read(lora_serial,0, &lora_char, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量释放后再次读取数据 */
           rt_sem_take(&lora_sem, RT_WAITING_FOREVER);
        }
        // 步进电机控制指令
        if(lora_char=='S')
        {
            step_buffer[step_buffer_index]=lora_char;
            step_buffer_index+=1;
        }
        else if (step_buffer_index !=0) {
            step_buffer[step_buffer_index]=lora_char;
            step_buffer_index+=1;
        }
        //如果接收到完整数据后分割字符串处理
        if(lora_char=='Z'&&step_buffer_index!=0)
        {
            step_buffer[step_buffer_index] = '\0';
            sscanf(step_buffer,"S%[0-9],%[0-9]Z",step1_s,step2_s);
            step_motor[0] = atoi(step1_s);//转弯
            step_motor[1] = atoi(step2_s);//直行
          rt_kprintf("step_motor0: %d step_motor1:  %d\r\n",step_motor[0],step_motor[1]);
            step_buffer_index =0;
        }
        // 步进电机控制指令
        if(lora_char=='L')
        {
            line_buffer[line_buffer_index]=lora_char;
            line_buffer_index+=1;
        }
        else if (line_buffer_index !=0) {
            line_buffer[line_buffer_index]=lora_char;
            line_buffer_index+=1;
        }
        //如果接收到完整数据后分割字符串处理
        if(lora_char=='Z'&&line_buffer_index!=0)
        {
            line_buffer[line_buffer_index] = '\0';
            sscanf(line_buffer,"L%[0-9]Z",line_s);
            line_step_motor = atoi(line_s);// 拉线步进
            rt_kprintf("line_step_motor: %d \r\n",line_step_motor);
            line_buffer_index =0;
        }
     //运动pwm电机控制指令
        if (lora_char == 'P')
        {
            pwm_buffer[pwm_buffer_index] = lora_char;
            pwm_buffer_index+=1;
        }
        else if (pwm_buffer_index != 0) {
            pwm_buffer[pwm_buffer_index] = lora_char;
            pwm_buffer_index+=1;
        }
              // 接收到完整数据 后分隔字符串处理
        if (lora_char == 'Z' && pwm_buffer_index!=0) {
            pwm_buffer[pwm_buffer_index] = '\0';
              sscanf(pwm_buffer,"P%[0-9],%[0-9]Z",pwm1_s,pwm2_s);
              pwm_motor[0] = atoi(pwm1_s);//直行
              pwm_motor[1] = atoi(pwm2_s);//转弯
            rt_kprintf("pwm_motor 0 %d pwm_motor 1  %d\r\n",pwm_motor[0],pwm_motor[1]);
              pwm_buffer_index =0;
        }

    }
}

/* 线程 */
int thread_lora_24(void)
{
    init_lora();
    /* 创建线程 */
    thread_lora_rec = rt_thread_create("lora_rec",
            lora_rec, RT_NULL,
                            THREAD_STACK_SIZE,
                            1, THREAD_TIMESLICE);
    /* 如果获得线程控制块，启动这个线程 */
    if (thread_lora_rec != RT_NULL)
        rt_thread_startup(thread_lora_rec);

    /* 创建线程 */
//    thread_lora_send = rt_thread_create("lora_send",
//        lora_send, RT_NULL,
//                                1025,
//                                5, THREAD_TIMESLICE);
    /* 如果获得线程控制块，启动这个线程 */
//    if (thread_lora_send != RT_NULL)
//        rt_thread_startup(thread_lora_send);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_lora_24, thread_lora_24);

