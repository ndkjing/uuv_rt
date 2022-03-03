/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */

#include "led.h"
#include "relay.h"
#define THREAD_PRIORITY         3
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

void control_led(int j1)
{
    if(j1==1)
    {
        rt_pin_write(LED1, PIN_LOW);
    }
    else if(j1==2) {
        rt_pin_write(LED1, PIN_HIGH);
    }
}
/* 线程 */
void init_led(void)
{
    //设置管脚的模式
    rt_pin_mode(LED1, PIN_MODE_OUTPUT);
    //初始化电平
    rt_pin_write(LED1, PIN_HIGH);
}
