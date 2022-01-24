/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#ifndef APPLICATIONS_LORA_24_H_
#define APPLICATIONS_LORA_24_H_


#include <rtthread.h>
#include "rtconfig.h"
#include <rtdevice.h>
#include "drv_common.h"

int thread_lora_24(void);
void send_data(char* buf,int len);

#endif /* APPLICATIONS_LORA_24_H_ */
