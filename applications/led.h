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
#define LED1         GET_PIN(E, 13)  //1
#define LED2        GET_PIN(E, 11)  //2
#define LED3         GET_PIN(E, 9)  //3
//lora模式引脚
#define MD0        GET_PIN(A, 0)  //2
#define MD1         GET_PIN(A, 1)  //3
// 外部状态的引脚
#define RED_LED         GET_PIN(C, 0)  //1    红色
#define BEEP_LED        GET_PIN(C, 1)  //2    蜂鸣器
#define YELLOW_LED         GET_PIN(C, 2)  //3   黄色
#define GREEN_LED        GET_PIN(C, 3)  //2     绿色
//void RBG_light(int a);
void control_status_led(int status_code);
void control_out_status_led(int status_code);
void init_led(void);


#endif /* APPLICATIONS_LED_H_ */
