/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#ifndef APPLICATIONS_MANAGER_H_
#define APPLICATIONS_MANAGER_H_
#include <rtthread.h>
#include "rtconfig.h"
#include <rtdevice.h>
#include "drv_common.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

#include "relay.h"
#include "led.h"
#include "motor_pwm.h"
#include "back_home.h"
#include "lora_24.h"
#include "step_motor.h"
extern u_int relay_status;
extern u_int row;// 横向摇杆接受值0-99
extern u_int col;// 纵向摇杆接受值0-99
extern u_int led_status;    //   当前状态灯编码  0 绿色  1 黄色  2 红色  3 蜂鸣
extern u_int current_theta; //当前船头角度
extern float_t dump_energy;  //  当前剩余电量
extern float_t back_home_erergy;  //  返航电量阈值
extern int current_status;//  当前灯状态
extern double lng;  // 经度
extern double lat;   // 纬度
extern double target_longitude;  // 目标经度
extern double target_latitude ;   // 目标纬度
extern int b_back_home;
extern bool b_limit_switch_1;  // 限位开关1
extern bool b_limit_switch_2; // 限位开关2
extern bool b_limit_switch_3; // 限位开关3
extern bool b_limit_switch_4;  // 限位开关4
//extern u_int targe_theta; //目标角
extern double angle_x;//  x轴角度
extern double angle_y;
extern int step_motor[2]; //步进电机运动指令 1 正转  2 反转  3 停止
extern int pwm_motor[2]; //主侧电机运动指令
extern int line_step_motor;  //拉线步进
extern int relay1;  //继电器
extern double deep;  // 深度
extern int tem;  // 温度
int thread_manager(void);
#endif /* APPLICATIONS_MANAGER_H_ */
