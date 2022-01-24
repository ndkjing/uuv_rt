/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
 /* 控制继电器  单次触发和延时翻转（水泵）*/

#ifndef APPLICATIONS_RELAY_H_
#define APPLICATIONS_RELAY_H_
#include <rtthread.h>
#include <rtdevice.h>
#include  <stdbool.h>
#include "drv_common.h"
#include "manager.h"
#define DRAW_PIN        GET_PIN(E, 6)  //抽水继电器管脚
#define BACK_PIN         GET_PIN(E, 5)  //备用继电器管脚
void init_relay();
void set_relay(bool a);
void set_relay_timing(bool a);
void delete_time();
#endif /* APPLICATIONS_RELAY_H_ */
