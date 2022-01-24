/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 * gps
 */
#include "gps.h"
#include "manager.h"



static rt_thread_t t_gps = RT_NULL;
rt_device_t gps_dev;
struct rt_semaphore gps_sem;
rt_size_t rx_len = 0;  // 本次接收数据长度
rt_int32_t last_rx_len = 0;   // 上次接收数据长度
rt_int32_t is_back_flag=1;  //开机接收到GPS就确定返航点
struct serial_configure u3_configs = RT_SERIAL_CONFIG_DEFAULT;

rt_err_t rx_gps_callback(rt_device_t dev_, rt_size_t size_)
{
    rx_len = size_;
    rt_sem_release(&gps_sem);
    return RT_EOK;
}

void check_gps(void *parameter)
    {
        char g_char;
        int gps_index = 0;  // GPS数据
        char gps_buffer[128] = {0};  // GPS数据
        char lng_s_int[15]={0};
        char lat_s_int[15]={0};
        char lng_s_float[15]={0};
        char lat_s_float[15]={0};
//        char temp_s[15]={0};
        while (1)
        {
            while(rt_device_read(gps_dev, 0, &g_char, 1) != 1)//等待接收完
            {
               rt_sem_take(&gps_sem, RT_WAITING_FOREVER);//释放信息量
            }
//            rt_kprintf("gps :%c\n",g_char);
            if (g_char=='$')
            {
                gps_buffer[gps_index]=g_char;
                gps_index+=1;
            }
            else if (gps_index!=0)
            {
                gps_buffer[gps_index]=g_char;
                gps_index+=1;
            }
            if (g_char == 'E' && gps_index!=0)
            {
                gps_buffer[gps_index] = '\0';
                sscanf(gps_buffer,"$GPRMC,%*[^,],A,%2s%[^,],N,%3s%[^,],E",lat_s_int,lat_s_float,lng_s_int,lng_s_float);
//                rt_kprintf("lng_s:%s,lat_s:%s  %s  %s\n",lat_s_int,lat_s_float,lng_s_int,lng_s_float);
                //度分转度格式
               lng = atof(lng_s_int)+atof(lng_s_float)/60.000000;
               lat = atof(lat_s_int)+atof(lat_s_float)/60.000000;
               if(is_back_flag==1&&atoi(lng_s_int)>10&&atoi(lat_s_int)>10)
               {
                   is_back_flag=0;
                   target_longitude=lng;
                   target_latitude=lat;
                   rt_kprintf("lng %d lat %d  %d  %d\n",(int)(lng*1000000),(int)(lat*1000000),(int)(target_longitude*1000000),(int)(target_latitude*1000000));

               }
                gps_index =0;
             }
        }
    }

int init_gps()
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
    u3_configs.baud_rate = BAUD_RATE_115200;        //修改波特率为 115200
    u3_configs.data_bits = DATA_BITS_8;           //数据位 8
    u3_configs.stop_bits = STOP_BITS_1;           //停止位 1
    u3_configs.bufsz     = 64;                   //修改缓冲区 buff size 为512
    u3_configs.parity    = PARITY_NONE;           //无奇偶校验位
    rt_device_control(gps_dev, RT_DEVICE_CTRL_CONFIG, (void *)&u3_configs);

    rt_device_set_rx_indicate(gps_dev, rx_gps_callback);

    ret = rt_sem_init(&gps_sem,"rx_sem", 0, RT_IPC_FLAG_FIFO);
    if(ret < 0){
        LOG_E("rt_sem_init failed[%d]...\n",ret);
        return ret;
    }
    return 0;
}

void thread_gps()
{
    init_gps();
    /* 创建线程 */
        t_gps = rt_thread_create("thread_gps",
                                check_gps,
                                RT_NULL,
                                2048,
                                10,
                                5);

        /* 如果获得线程控制块，启动这个线程 */
        if (t_gps != RT_NULL)
        {
            rt_thread_startup(t_gps);
        }
}

