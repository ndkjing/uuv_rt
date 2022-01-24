/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-04     jing       the first version
 */
#include "step_motor.h"
static rt_thread_t tid_step = RT_NULL;
static rt_thread_t tid_limit = RT_NULL;
int target_step1=3; //目标电机
int target_dir1=3;//目标电机运动方向
int target_step2=3; //目标电机
int target_dir2=3;//目标电机运动方向    1:推   2:拉   3:不转
int target_line_step=3; //目标电机
int target_line_dir=3;//目标电机运动方向    1:推   2:拉   3:不转
//int target_line_dir=3;  // 拉线电机转动
void init_motor()
{
    rt_pin_mode(dir1, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(pul1, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(dir2, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(pul2, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(limit_switch_1, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(limit_switch_2, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(limit_switch_3, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(limit_switch_4, PIN_MODE_INPUT_PULLUP);
    //初始化电平
    rt_pin_write(dir1, PIN_LOW);
    rt_pin_write(pul1, PIN_LOW);
    rt_pin_write(dir2, PIN_LOW);
    rt_pin_write(pul2, PIN_LOW);
}
/*
 * 读取4个限位开关
 * */
void read_limit_switch()
{
    while (1)
    {
        if (rt_pin_read(limit_switch_1)==PIN_LOW)
        {
             b_limit_switch_1=false;
        }
        else
        {
            b_limit_switch_1=true;
        }
        if (rt_pin_read(limit_switch_2)==PIN_LOW)
        {
             b_limit_switch_2=false;
        }
        else
        {
            b_limit_switch_2=true;
        }
        if (rt_pin_read(limit_switch_3)==PIN_LOW)
        {
             b_limit_switch_3=false;
        }
        else
        {
            b_limit_switch_3=true;
        }
        if (rt_pin_read(limit_switch_4)==PIN_LOW)
        {
             b_limit_switch_4=false;
        }
        else
        {
            b_limit_switch_4=true;
        }
        rt_thread_mdelay(50);
    }

}

/*
 * 根据控制命令设置步进电机运动控制上升,下降,平衡模式
 * */
void loop_step_motor()
{
    int last_target_dir1=3;
    int last_target_dir2=3;
    int wait_time=10;
    int last_line_dir=3;
    while (1)
    {
        if (target_step1==1)
        {
            if (target_dir1==2 &&target_dir1!=last_target_dir1)
            {
                rt_pin_write(dir1, PIN_LOW);
                rt_thread_mdelay(50);
                last_target_dir1=target_dir1;
            }
            else if (target_dir1==1&&target_dir1!=last_target_dir1){
                rt_pin_write(dir1, PIN_HIGH);
                rt_thread_mdelay(50);
                last_target_dir1=target_dir1;
            }
            if (b_limit_switch_1==false||b_limit_switch_2==false)
            {
                target_dir1=3;
            }
            if (target_dir1==1||target_dir1==2)
            {
                rt_pin_write(pul1, PIN_LOW);
                rt_thread_mdelay(wait_time);
                rt_pin_write(pul1, PIN_HIGH);
                rt_thread_mdelay(wait_time);
            }
       }
        if (target_step2==1)
        {
            if (target_dir2==2&&target_dir2!=last_target_dir2)
            {
                rt_pin_write(dir2, PIN_LOW);
                rt_thread_mdelay(50);
                last_target_dir2=target_dir2;
            }
            else if (target_dir2==1&&target_dir2!=last_target_dir2){
                rt_pin_write(dir2, PIN_HIGH);
                rt_thread_mdelay(50);
                last_target_dir2=target_dir2;
            }

            if (b_limit_switch_3==false||b_limit_switch_4==false)
            {
                target_dir2=3;
            }
            if (target_dir1==1||target_dir1==2)
            {
                rt_pin_write(pul2, PIN_LOW);
                rt_thread_mdelay(wait_time);
                rt_pin_write(pul2, PIN_HIGH);
                rt_thread_mdelay(wait_time);
            }
        }
        if (target_dir2==3 || target_dir1==3)
        {
            rt_thread_mdelay(10);
        }
    }
    if (target_line_dir==2&&target_line_dir!=last_line_dir)
        {
            rt_pin_write(dir_line, PIN_LOW);
            rt_thread_mdelay(50);
            last_target_dir2=target_dir2;
        }
        else if (target_dir2==1&&target_dir2!=last_target_dir2){
            rt_pin_write(dir_line, PIN_HIGH);
            rt_thread_mdelay(50);
            last_target_dir2=target_dir2;
        }
    if (target_line_dir==1||target_line_dir==2)
    {
        rt_pin_write(pul_line, PIN_LOW);
        rt_thread_mdelay(wait_time);
        rt_pin_write(pul_line, PIN_HIGH);
        rt_thread_mdelay(wait_time);
    }
}

void set_step_motor(int target_step1_,int target_step2_)
{
    if (target_step1_==1)
    {
        target_step1 = 1;
        target_dir1 = 1;
        printf("target_dir1 %d\n",target_dir1);
    }
    else if (target_step1_==2)
    {
        target_step1 = 1;
        target_dir1 = 2;
        printf("target_dir1 %d\n",target_dir1);
    }
    else if (target_step1_==3)
    {
        target_step1 = 1;
        target_dir1 = 3;
    }
    if (target_step2_==1)
    {
        target_step2 = 1;
        target_dir2 = 1;
    }
    else if (target_step2_==2)
    {
        target_step2 = 1;
        target_dir2 = 2;
    }
    else if (target_step2_==3)
    {
        target_step2 = 1;
        target_dir2 = 3;
    }
}

//设置拉线电机
void set_line_step_motor(int target_step)
{
    target_line_dir = target_step;
}

// 循环一直修改pwm值让当前输出等于目标值
int thread_setp()
{
    rt_err_t ret = RT_EOK;
    init_motor();
    /* 创建线程 */
    tid_limit = rt_thread_create("t_limit",read_limit_switch, RT_NULL, 1024, 0, 10);
   /* 创建成功则启动线程 */
   if (tid_limit != RT_NULL)
   {
       rt_thread_startup(tid_limit);
   }
   else
   {
       ret = RT_ERROR;
   }
   /* 创建线程 */
   tid_step = rt_thread_create("t_step_motor",loop_step_motor, RT_NULL, 1024, 0, 10);
  /* 创建成功则启动线程 */
  if (tid_step != RT_NULL)
  {
      rt_thread_startup(tid_step);
  }
   return ret;
}
