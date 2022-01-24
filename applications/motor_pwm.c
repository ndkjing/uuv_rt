/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#include "motor_pwm.h"
#include "manager.h"

#define PWM_DEV_NAME        "pwm4"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL1   1   //PB6
#define PWM_DEV_CHANNEL2   2   //PB7
//#define PWM_DEV_CHANNEL3   3
//#define PWM_DEV_CHANNEL4   4
struct rt_device_pwm      *pwm_dev;

rt_uint32_t period= 20000000; /* 周 期 为20ms/50hz， 单 位 为 纳 秒ns */
rt_uint32_t pulse=  1500000; /* PWM脉 冲 宽 度 值， 单 位 为 纳秒ns   1000000 */
rt_uint32_t current_left_pwm =1500;
rt_uint32_t current_right_pwm =1500;
rt_uint32_t target_left_pwm =1500;
rt_uint32_t target_right_pwm =1500;

static rt_thread_t tid_pwmled = RT_NULL;


void init_motor_pwm()
{
//    SystemClock_Config();

    /* 查 找 设 备 */

     pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
     if (pwm_dev == RT_NULL)
     {
         rt_kprintf("pwm sample run failed! can't find pwm4!\n");
     }
     rt_kprintf("pwm sample run ! find pwm4!\n");
     /* 使 能 设 备 */
         rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL1);
         rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL2);
     /* 设 置PWM周 期 和 脉 冲 宽 度 默 认 值 */
      rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL1, period, pulse);
      rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL2, period, pulse);
}

// 循环一直修改pwm值让当前输出等于目标值
void loop_change_pwm(void *parameter)
{
    rt_uint32_t change_pwm_ceil = 2;

    while (1)
    {
        if((current_left_pwm - target_left_pwm != 0) || (current_right_pwm - target_right_pwm != 0))
        {
            if (current_left_pwm!=target_left_pwm)
            {
                if (current_left_pwm>target_left_pwm)
                {
                    current_left_pwm -= change_pwm_ceil;
                }
                else
                {
                    current_left_pwm += change_pwm_ceil;
                }
                rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL1, period, current_left_pwm*1000);  /* 设 置PWM周 期 和 脉 冲 宽 度 */
            }
            if (current_right_pwm!=target_right_pwm)
                {
                    if (current_right_pwm>target_right_pwm)
                    {
                        current_right_pwm -= change_pwm_ceil;
                    }
                    else
                    {
                        current_right_pwm += change_pwm_ceil;
                    }
                    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL2, period, current_right_pwm*1000);/* 设 置PWM周 期 和 脉 冲 宽 度 */
                }
            rt_thread_mdelay(2);
        }
        else
        {
//            rt_kprintf("current_left_pwm:%d ,current_right_pwm:%d target_left_pwm:%d ,target_right_pwm:%d \r\n",current_left_pwm,current_right_pwm,target_left_pwm,target_right_pwm);
            rt_thread_mdelay(10);
        }
    }
}

//设置目标pwm值
void set_pwm(u_int left_pwm,u_int right_pwm)
{
    target_left_pwm  = left_pwm;
    target_right_pwm = right_pwm;
}

// 循环一直修改pwm值让当前输出等于目标值
int thread_pwm()
{
    rt_err_t ret = RT_EOK;
    MX_TIM4_Init();
    init_motor_pwm();
    /* 创建 serial 线程 */
    tid_pwmled = rt_thread_create("t_pwmled",loop_change_pwm, RT_NULL, 1024, 0, 10);
   /* 创建成功则启动线程 */
   if (tid_pwmled != RT_NULL)
   {
       rt_thread_startup(tid_pwmled);
   }
   else
   {
       ret = RT_ERROR;
   }
   return ret;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(loop_change_pwm, pwm sample);
