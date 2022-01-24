/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 * adc采集获取电池剩余电量
 */
#ifndef APPLICATIONS_ENERGY_ADC_H_
#define APPLICATIONS_ENERGY_ADC_H_

#include <rtthread.h>
#include "rtconfig.h"
#include <rtdevice.h>
#include "drv_common.h"
#include "manager.h"

void thread_energy();

#endif /* APPLICATIONS_ENERGY_ADC_H_ */
