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


// 控制板子状态灯
void control_status_led(int status_code)
{
    switch(status_code)
    {
        //绿色
    case 1:
        rt_pin_write(LED1, PIN_LOW);
        rt_pin_write(LED2, PIN_HIGH);
        rt_pin_write(LED3, PIN_LOW);
        break;
        //红色
    case 2:
        rt_pin_write(LED1, PIN_HIGH);
        rt_pin_write(LED2, PIN_LOW);
        rt_pin_write(LED3, PIN_LOW);
        break;
        //蓝色
    case 3:
        rt_pin_write(LED1, PIN_LOW);
        rt_pin_write(LED2, PIN_LOW);
        rt_pin_write(LED3, PIN_HIGH);
        break;
    default:
        rt_pin_write(LED1, PIN_LOW);
        rt_pin_write(LED2, PIN_LOW);
        rt_pin_write(LED3, PIN_LOW);
        break;
    }
}


// 控制船外部状态灯  1  绿色 2 红色  3黄色   4 蜂鸣  其他   关
void control_out_status_led(int status_code)
{
    switch(status_code)
    {
        //绿色
    case 1:
        rt_pin_write(RED_LED, PIN_LOW);
        rt_pin_write(BEEP_LED, PIN_LOW);
        rt_pin_write(YELLOW_LED, PIN_LOW);
       rt_pin_write(GREEN_LED, PIN_HIGH);
        break;
        //红色
    case 2:
        rt_pin_write(RED_LED, PIN_HIGH);
        rt_pin_write(BEEP_LED, PIN_LOW);
        rt_pin_write(YELLOW_LED, PIN_LOW);
       rt_pin_write(GREEN_LED, PIN_LOW);
        break;
        //黄色
    case 3:
        rt_pin_write(RED_LED, PIN_LOW);
        rt_pin_write(BEEP_LED, PIN_LOW);
        rt_pin_write(YELLOW_LED, PIN_HIGH);
       rt_pin_write(GREEN_LED, PIN_LOW);
       break;
       //蜂鸣
    case 4:
            rt_pin_write(RED_LED, PIN_LOW);
            rt_pin_write(BEEP_LED, PIN_LOW);
            rt_pin_write(YELLOW_LED, PIN_LOW);
           rt_pin_write(GREEN_LED, PIN_LOW);
           break;
    default:
        rt_pin_write(RED_LED, PIN_LOW);
        rt_pin_write(BEEP_LED, PIN_LOW);
        rt_pin_write(YELLOW_LED, PIN_LOW);
       rt_pin_write(GREEN_LED, PIN_LOW);
        break;
    }
}

/* 线程 */
void init_led(void)
{
    //设置管脚的模式
    rt_pin_mode(LED1, PIN_MODE_OUTPUT);
    rt_pin_mode(LED2, PIN_MODE_OUTPUT);
    rt_pin_mode(LED3, PIN_MODE_OUTPUT);
    rt_pin_mode(MD0, PIN_MODE_OUTPUT);
    rt_pin_mode(MD1, PIN_MODE_OUTPUT);
    rt_pin_mode(RED_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(BEEP_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(YELLOW_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(GREEN_LED, PIN_MODE_OUTPUT);
    //初始化电平
    rt_pin_write(MD0, PIN_LOW);
    rt_pin_write(MD1, PIN_LOW);
    rt_pin_write(RED_LED, PIN_LOW);
    rt_pin_write(BEEP_LED, PIN_LOW);
    rt_pin_write(YELLOW_LED, PIN_LOW);
    rt_pin_write(GREEN_LED, PIN_LOW);

}
