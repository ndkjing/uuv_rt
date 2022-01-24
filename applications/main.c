/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <stdio.h>
#include "lora_24.h"
#include "led.h"
#include "manager.h"
#include "gps.h"
#include "motor_pwm.h"
#include "compass.h"
#include "energy_adc.h"
#include "relay.h"
#include "back_home.h"
#include "step_motor.h"
#include "imu.h"
#include "uart_deep.h"
//#include "iic_deep.h"
int main(void)
{
//    init_relay();
//    init_led();
    thread_pwm();
    thread_deep();
    thread_lora_24();
    thread_manager();
    thread_compass();
//    thread_energy();
    thread_setp();
    thread_imu();
//    set_step_motor(1,1);
//    rt_thread_mdelay(2000);
//    set_step_motor(2,1);
//    i2c_aht10_sample();
    return RT_EOK;
}
