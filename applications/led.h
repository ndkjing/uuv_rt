/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#ifndef APPLICATIONS_LED_H_
#define APPLICATIONS_LED_H_
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
// 板子状态灯的引脚
#define LED1         GET_PIN(C, 15)  //1
void control_led(int j1);
void init_led(void);


#endif /* APPLICATIONS_LED_H_ */
