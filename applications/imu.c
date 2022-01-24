/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-04     jing       the first version
 */

#include "imu.h"
#include "manager.h"



static rt_thread_t t_imu = RT_NULL;
rt_device_t imu_dev;
struct rt_semaphore imu_sem;


struct serial_configure imu_u5_configs = RT_SERIAL_CONFIG_DEFAULT;

rt_err_t rx_imu_callback(rt_device_t dev_, rt_size_t size_)
{
    rt_sem_release(&imu_sem);
    return RT_EOK;
}

void check_imu(void *parameter)
    {
        unsigned char i_char;
        int imu_index = 0;  // 罗盘数据
        int imu_datatype = 3;  // 0 加速度包  1  角速度包   2  角度包
        unsigned char imu_buffer[11] = {0};
        //Magx:-1863,Magy:28,Magz:2565,Yaw:179.1
        while (1)
        {
            while(rt_device_read(imu_dev, 0, &i_char, 1) != 1)//等待接收完
            {
               rt_sem_take(&imu_sem, RT_WAITING_FOREVER);//释放信息量
            }
//            rt_kprintf("%c",g_char);
//            rt_kprintf("imu_datatype: %d  imu_index: %d",imu_datatype,imu_index);
            if (i_char==0x55)
            {
                imu_index=0;
                imu_buffer[imu_index]=(unsigned char)i_char;
                imu_index+=1;
                imu_datatype=3;
            }
            if (imu_index==1&&i_char==0x53)
            {
                imu_datatype=2;
                imu_buffer[imu_index]=(unsigned char)i_char;
                imu_index+=1;
//                rt_kprintf("%s\n",imu_buffer);
            }
//            if(imu_buffer[0]==0x55&&g_char!=0x53&&imu_index==1){
//                imu_index =0;
//                imu_datatype=3;
//            }
            else if (imu_index!=0&&imu_datatype==2)
            {
                imu_buffer[imu_index]=(unsigned char)i_char;
                imu_index+=1;
            }
            if (imu_index==11&&imu_datatype==2)
            {
              // 判断是否校验成功
                if ((unsigned char)(imu_buffer[0]+imu_buffer[1]+imu_buffer[2]+imu_buffer[3]+imu_buffer[4]+imu_buffer[5]+imu_buffer[6]+imu_buffer[7]+imu_buffer[8]+imu_buffer[9])==(unsigned char)imu_buffer[10])
                {
                    angle_x = (double)((int)imu_buffer[3]*256+(int)imu_buffer[2])/32768.0*180;
                    angle_y  = (double)(imu_buffer[5]*256+imu_buffer[4])/32768.0*180;
                    double tempurate = (double)(imu_buffer[9]*256+imu_buffer[8])/340.0+36.53;
//                    printf("Anglex = %d  y %d   T=%d\r\n",(int)angle_x,(int)angle_y,(int)tempurate);
                }
//                else {
//                    printf("valid fail\n");
//                }
                imu_index =0;
                imu_datatype=3;
//
             }
        }
    }

int init_imu()
{
    rt_err_t ret = 0;
    imu_dev = rt_device_find("uart5");
    if(imu_dev == RT_NULL){
        return -EINVAL;
    }
    ret = rt_device_open(imu_dev, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);  //中断
    if(ret < 0){
        rt_kprintf("ret : %d\n",ret);
        return ret;
    }
    /* step2：修改串口配置参数 */
    imu_u5_configs.baud_rate = BAUD_RATE_115200;        //修改波特率为 115200(不能9600，鬼知道为啥)
    imu_u5_configs.data_bits = DATA_BITS_8;           //数据位 8
    imu_u5_configs.stop_bits = STOP_BITS_1;           //停止位 1
    imu_u5_configs.bufsz     = 64;                   //修改缓冲区 buff size 为512
    imu_u5_configs.parity    = PARITY_NONE;           //无奇偶校验位
    rt_device_control(imu_dev, RT_DEVICE_CTRL_CONFIG, (void *)&imu_u5_configs);

    rt_device_set_rx_indicate(imu_dev, rx_imu_callback);

    ret = rt_sem_init(&imu_sem,"rx_sem", 0, RT_IPC_FLAG_FIFO);
    if(ret < 0){
        return ret;
    }
    return 0;
}

void thread_imu()
{
    init_imu();
    /* 创建线程 */
        t_imu = rt_thread_create("thread_imu",
                check_imu,
                                RT_NULL,
                                2048,
                                10,
                                5);

        /* 如果获得线程控制块，启动这个线程 */
        if (t_imu != RT_NULL)
        {
            rt_thread_startup(t_imu);
        }
}

