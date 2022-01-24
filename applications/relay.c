/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#include "relay.h"

bool a = true;
/* 定时器的控制块 */
static rt_timer_t relay_timer;
void set_relay(bool a)
{
    if (a)
    {
        rt_pin_write(DRAW_PIN, PIN_LOW);
    }
    else {
        rt_pin_write(DRAW_PIN, PIN_HIGH);
    }
}

void start_draw(void *parameter)
{
    rt_pin_write(DRAW_PIN, PIN_HIGH);
    rt_kprintf("DRAWing \n");
    current_status=1;
    rt_thread_mdelay(50);
}

void set_relay_timing(bool a)
{
    if (a)
    {
        /* 创建定时器 单次定时器 */
        relay_timer = rt_timer_create("relay_timer", start_draw,
                                     RT_NULL,  5000,
                                     RT_TIMER_FLAG_ONE_SHOT);
            /* 启动定时器 */
            if (relay_timer != RT_NULL)
            {
                rt_timer_start(relay_timer);
                relay_status=0;
            }


    }
//    else {
//      rt_timer_stop(relay_timer);
//    }
}

void delete_time()
{
    rt_timer_stop(relay_timer);
}
void init_relay()
{
    rt_pin_mode(BACK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(DRAW_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(BACK_PIN, PIN_HIGH);
    rt_pin_write(DRAW_PIN, PIN_HIGH);
}





