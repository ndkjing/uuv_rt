/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-13     jing       the first version
 */
#include "uart_deep.h"
#include "manager.h"



static rt_thread_t t_deep = RT_NULL;
rt_device_t gps_dev;
struct rt_semaphore gps_sem;
rt_size_t _rx_len_deep = 0;  // 本次接收数据长度
rt_int32_t last_rx_len_deep = 0;   // 上次接收数据长度
struct serial_configure u3_configs_deep = RT_SERIAL_CONFIG_DEFAULT;

rt_err_t rx_deep_callback(rt_device_t dev_, rt_size_t size_)
{
    _rx_len_deep = size_;
    rt_sem_release(&gps_sem);
    return RT_EOK;
}

void check_deep(void *parameter)
    {
        char g_char;
        int gps_index = 0;  // GPS数据
        char gps_buffer[128] = {0};  // GPS数据
        char deep_s_int[15]={0};
        char tem_s_int[15]={0};
//        char temp_s[15]={0};
        while (1)
        {
            while(rt_device_read(gps_dev, 0, &g_char, 1) != 1)//等待接收完
            {
               rt_sem_take(&gps_sem, RT_WAITING_FOREVER);//释放信息量
            }
//            rt_kprintf("gps :%c\n",g_char);
            if (g_char=='T')
            {
                gps_buffer[gps_index]=g_char;
                gps_index+=1;
            }
            else if (gps_index!=0)
            {
                gps_buffer[gps_index]=g_char;
                gps_index+=1;
            }
            if (g_char == '\r' && gps_index!=0)
            {
//                rt_kprintf("rec deep:%s",gps_buffer);
                gps_buffer[gps_index] = '\0';
                sscanf(gps_buffer,"T=%[^D]D=%[^\r]",tem_s_int,deep_s_int);
//                rt_kprintf("deep_s:%s ,tem_s:%s\n",deep_s_int,tem_s_int);
//                //转为浮点数格式
               deep = atof(deep_s_int);
               tem = atoi(tem_s_int);
//               rt_kprintf("deep_s_int:%d ,tem_s_int:%d\n",(int)deep,tem);
                gps_index =0;
             }
        }
    }

int init_deep()
{
    rt_err_t ret = 0;
    gps_dev = rt_device_find("uart3");
    if(gps_dev == RT_NULL){
        LOG_E("rt_device_find[uart3] failed...\n");
        return -EINVAL;
    }

//    ret = rt_device_open(gps_dev, RT_DEVICE_FLAG_DMA_RX); //DMA
    ret = rt_device_open(gps_dev, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);  //中断
    if(ret < 0){
        LOG_E("rt_device_open[uart3] failed...\n");
        rt_kprintf("ret : %d\n",ret);
        return ret;
    }
    /* step2：修改串口配置参数 */
    u3_configs_deep.baud_rate = BAUD_RATE_115200;        //修改波特率为 115200
    u3_configs_deep.data_bits = DATA_BITS_8;           //数据位 8
    u3_configs_deep.stop_bits = STOP_BITS_1;           //停止位 1
    u3_configs_deep.bufsz     = 64;                   //修改缓冲区 buff size 为512
    u3_configs_deep.parity    = PARITY_NONE;           //无奇偶校验位
    rt_device_control(gps_dev, RT_DEVICE_CTRL_CONFIG, (void *)&u3_configs_deep);

    rt_device_set_rx_indicate(gps_dev, rx_deep_callback);

    ret = rt_sem_init(&gps_sem,"rx_sem", 0, RT_IPC_FLAG_FIFO);
    if(ret < 0){
        LOG_E("rt_sem_init failed[%d]...\n",ret);
        return ret;
    }
    return 0;
}

void thread_deep()
{
    init_deep();
    /* 创建线程 */
    t_deep = rt_thread_create("thread_deep",
                                check_deep,
                                RT_NULL,
                                2048,
                                10,
                                5);

        /* 如果获得线程控制块，启动这个线程 */
        if (t_deep != RT_NULL)
        {
            rt_thread_startup(t_deep);
        }
}
