/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-04     jing       the first version
 * 驱动步进电机
 */
#ifndef APPLICATIONS_STEP_MOTOR_H_
#define APPLICATIONS_STEP_MOTOR_H_
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "manager.h"
#define pul1        GET_PIN(D, 11)  //2
#define dir1         GET_PIN(D, 12)  //3
#define pul2        GET_PIN(D, 13)  //2
#define dir2         GET_PIN(D, 14)  //3
#define pul_line        GET_PIN(D, 10)  //2
#define dir_line         GET_PIN(D, 15)  //3
#define limit_switch_1        GET_PIN(C, 6)  //限位开关1
#define limit_switch_2        GET_PIN(C, 7)  //限位开关2
#define limit_switch_3        GET_PIN(D, 8)  //限位开关3
#define limit_switch_4        GET_PIN(D, 9)  //限位开关4
int thread_setp();
#endif /* APPLICATIONS_STEP_MOTOR_H_ */
