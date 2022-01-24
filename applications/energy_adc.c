/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#include "energy_adc.h"

#define ADC_DEV_NAME        "adc1"      /* ADC 设备名称 */
#define ADC_DEV_CHANNEL     4           /* ADC 通道 */
#define REFER_VOLTAGE       322         /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */

rt_adc_device_t adc_dev;
static rt_thread_t t_energy = RT_NULL;
rt_uint32_t value=0, vol=0;
float_t Voltage;
int adc_init()
{
    rt_err_t ret = RT_EOK;

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }
    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);

    return ret;
}

static void check_dump_energy(void *parameter)
{
 while(1)
 {
     rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);
     /* 读取采样值 */
     value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
     /* 转换为对应电压值 */
     vol = value * REFER_VOLTAGE / CONVERT_BITS;
     Voltage=vol*17/200.0;
     dump_energy=(Voltage-22.2)/3.0*100;
     if (dump_energy<10) {
         dump_energy=0;
    }
     rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

//     rt_kprintf("vol:%d  voltage:%d  dump_energy:%d\n",vol,(int)Voltage,(int)dump_energy);
     rt_thread_mdelay(200);

 }
}


void thread_energy()
{
    adc_init();
    /* 创建线程 */
        t_energy = rt_thread_create("thread_energy",
                check_dump_energy, RT_NULL,
                                512,
                                16, 5);

        /* 如果获得线程控制块，启动这个线程 */
        if (t_energy != RT_NULL)
            rt_thread_startup(t_energy);
}


