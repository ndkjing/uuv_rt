/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#include "compass.h"
#include "manager.h"



static rt_thread_t t_compass = RT_NULL;
rt_device_t compass_dev;
struct rt_semaphore compass_sem;


struct serial_configure compass_u3_configs = RT_SERIAL_CONFIG_DEFAULT;

rt_err_t rx_compass_callback(rt_device_t dev_, rt_size_t size_)
{
    rt_sem_release(&compass_sem);
    return RT_EOK;
}

void check_compass(void *parameter)
    {
        char g_char;
        int compass_index = 0;  // 罗盘数据
        char compass_buffer[128] = {0};
        char compass_s[15]={0};
        //Magx:-1863,Magy:28,Magz:2565,Yaw:179.1
        while (1)
        {
            while(rt_device_read(compass_dev, 0, &g_char, 1) != 1)//等待接收完
            {
               rt_sem_take(&compass_sem, RT_WAITING_FOREVER);//释放信息量
            }
//            rt_kprintf("gps :%c\n",g_char);
            if (g_char=='Y')
            {
                compass_buffer[compass_index]=g_char;
                compass_index+=1;
            }
            else if (compass_index!=0)
            {
                compass_buffer[compass_index]=g_char;
                compass_index+=1;
            }
            if (g_char == '\n' && compass_index!=0)
            {
                compass_buffer[compass_index] = '\0';
//                rt_kprintf("compass_buffer_rec %s\n",compass_buffer);
                sscanf(compass_buffer,"Yaw:%s",compass_s);//Yaw:99.1
                //右手定则（正南0度）：180 -（-90）-0-90 ——>0-360
                current_theta = atoi(compass_s)+180;
                compass_index =0;
//                rt_kprintf("current_theta:%d\n",current_theta);

             }
        }
    }

int init_compass()
{
    rt_err_t ret = 0;
    compass_dev = rt_device_find("uart4");
    if(compass_dev == RT_NULL){
        return -EINVAL;
    }
    ret = rt_device_open(compass_dev, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);  //中断
    if(ret < 0){
        rt_kprintf("ret : %d\n",ret);
        return ret;
    }
    /* step2：修改串口配置参数 */
    compass_u3_configs.baud_rate = BAUD_RATE_115200;        //修改波特率为 115200(不能9600，鬼知道为啥)
    compass_u3_configs.data_bits = DATA_BITS_8;           //数据位 8
    compass_u3_configs.stop_bits = STOP_BITS_1;           //停止位 1
    compass_u3_configs.bufsz     = 64;                   //修改缓冲区 buff size 为512
    compass_u3_configs.parity    = PARITY_NONE;           //无奇偶校验位
    rt_device_control(compass_dev, RT_DEVICE_CTRL_CONFIG, (void *)&compass_u3_configs);

    rt_device_set_rx_indicate(compass_dev, rx_compass_callback);

    ret = rt_sem_init(&compass_sem,"rx_sem", 0, RT_IPC_FLAG_FIFO);
    if(ret < 0){
        return ret;
    }
    return 0;
}

void thread_compass()
{
    init_compass();
    /* 创建线程 */
        t_compass = rt_thread_create("thread_compass",
                check_compass,
                                RT_NULL,
                                2048,
                                10,
                                5);

        /* 如果获得线程控制块，启动这个线程 */
        if (t_compass != RT_NULL)
        {
            rt_thread_startup(t_compass);
        }
}

