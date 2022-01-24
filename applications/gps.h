/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 * 获取GPS数据：
 * 经度，纬度，速度
 */
#ifndef APPLICATIONS_GPS_H_
#define APPLICATIONS_GPS_H_

#include <rtthread.h>
#include "rtconfig.h"
#include <rtdevice.h>
#include "drv_common.h"
#include <rtdbg.h>
#include "manager.h"
#define DBG_LVL DBG_LOG
#define DBG_TAG "gps"
void thread_gps();

#endif /* APPLICATIONS_GPS_H_ */
