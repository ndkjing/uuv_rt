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
static rt_thread_t tid_step1 = RT_NULL;
static rt_thread_t tid_step2 = RT_NULL;
static rt_thread_t tid_limit = RT_NULL;
void init_motor()
{
    rt_pin_mode(dir1, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(pul1, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(dir2, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(pul2, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(dir_line, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(pul_line, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(limit_switch_1, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(limit_switch_2, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(limit_switch_3, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(limit_switch_4, PIN_MODE_INPUT_PULLUP);
    //初始化电平
    rt_pin_write(dir1, PIN_LOW);
    rt_pin_write(pul1, PIN_LOW);
    rt_pin_write(dir2, PIN_LOW);
    rt_pin_write(pul2, PIN_LOW);
    rt_pin_write(dir_line, PIN_LOW);
    rt_pin_write(pul_line, PIN_LOW);
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
    int last_target_dir1=3;  //步进电机1之前运动方向
    int last_target_dir2=3;  //步进电机2之前运动方向
    int step1_wait_time=10;  //步进电机1等待时间间隔
    int step1_min_wait_time=2;  //步进电机1最小等待时间间隔
    int step1_max_wait_time=10;  //步进电机1最大等待时间间隔
    int step1_decrease_count=10;  //步进电机1加速间隔次数
    while (1)
    {
        if (step_motor[0]!=1)
        {
            //判断微动开关
            if (b_limit_switch_1==false&&step_motor[0]==1)
            {
                step_motor[0]=3;
                continue;
            }
            else if (b_limit_switch_2==false&&step_motor[0]==2) {
                step_motor[0]=3;
                continue;
            }
            // 判断是否需要切换方向
            if (step_motor[0]==2 &&step_motor[0]!=last_target_dir1)
            {
                rt_pin_write(dir1, PIN_LOW);
                rt_thread_mdelay(50);
                last_target_dir1=step_motor[0];
                step1_wait_time=step1_max_wait_time;
            }
            else if (step_motor[0]==1&&step_motor[0]!=last_target_dir1){
                rt_pin_write(dir1, PIN_HIGH);
                rt_thread_mdelay(50);
                last_target_dir1=step_motor[0];
                step1_wait_time=step1_max_wait_time;
            }
            rt_pin_write(pul1, PIN_LOW);
            rt_thread_mdelay(step1_wait_time);
            rt_pin_write(pul1, PIN_HIGH);
            rt_thread_mdelay(step1_wait_time);
            if (step1_decrease_count>0)
                {
                    step1_decrease_count--;
                }
            else
            {
                step1_decrease_count=10;
                if (step1_wait_time>step1_min_wait_time)
                {
                    step1_wait_time--;
                }
                else
                {
                    step1_wait_time=step1_min_wait_time;
                }
            }
       }
        else {
            rt_thread_mdelay(10);
        }
    }
}


void loop_step_motor1()
{
    int last_target_dir2=3;  //步进电机2之前运动方向
    int step2_wait_time=10;     //步进电机1等待时间间隔
    int step2_min_wait_time=2;  //步进电机2最小等待时间间隔
    int step2_max_wait_time=10;  //步进电机1等待时间间隔
    int step2_decrease_count=10;  //步进电机2加速间隔次数
    while (1)
    {
        if (step_motor[1]==1)
        {
            //判断微动开关
            if (b_limit_switch_4==false&&step_motor[0]==1)
            {
                step_motor[1]=3;
                continue;
            }
            else if (b_limit_switch_3==false&&step_motor[0]==2) {
                step_motor[1]=3;
                continue;
            }
            // 判断是否需要切换方向
            if (step_motor[1]==2 &&step_motor[1]!=last_target_dir2)
            {
                rt_pin_write(dir2, PIN_LOW);
                rt_thread_mdelay(50);
                last_target_dir2=step_motor[1];
                step2_wait_time=step2_max_wait_time;
            }
            else if (step_motor[1]==1&&step_motor[1]!=last_target_dir2){
                rt_pin_write(dir2, PIN_HIGH);
                rt_thread_mdelay(50);
                last_target_dir2=step_motor[1];
                step2_wait_time=step2_max_wait_time;
            }
            rt_pin_write(pul2, PIN_LOW);
            rt_thread_mdelay(step2_wait_time);
            rt_pin_write(pul2, PIN_HIGH);
            rt_thread_mdelay(step2_wait_time);
            if (step2_decrease_count>0)
            {
                step2_decrease_count--;
            }
            else
            {
                step2_decrease_count=10;
                if (step2_wait_time>step2_min_wait_time)
                {
                    step2_wait_time--;
                }
                else
                {
                    step2_wait_time=step2_min_wait_time;
                }
            }
       }
       else
       {
            rt_thread_mdelay(10);
       }
    }
}


void loop_step_motor2()
{
    int step3_wait_time=10;   //步进电机3等待时间间隔
    int step3_min_wait_time=5;  //步进电机3最小等待时间间隔
    int step3_max_wait_time=10;  //步进电机3最大等待时间间隔
    int step3_decrease_count=10;  //步进电机3加速间隔次数
    int last_line_dir=3;    //步进电机3之前运动方向

    while (1)
    {
        if (line_step_motor!=3)
        {
       //拉线步进电机控制
        if (line_step_motor==2&&line_step_motor!=last_line_dir)
        {
            rt_pin_write(dir_line, PIN_LOW);
            rt_thread_mdelay(50);
            last_line_dir=line_step_motor;
            step3_wait_time=step3_max_wait_time;
        }
        else if (line_step_motor==1&&line_step_motor!=last_line_dir)
        {
            rt_pin_write(dir_line, PIN_HIGH);
            rt_thread_mdelay(50);
            last_line_dir=line_step_motor;
            step3_wait_time=step3_max_wait_time;
        }
        if (line_step_motor==1||line_step_motor==2)
        {
            rt_pin_write(pul_line, PIN_LOW);
            rt_thread_mdelay(step3_wait_time);
            rt_pin_write(pul_line, PIN_HIGH);
            rt_thread_mdelay(step3_wait_time);
            if (step3_decrease_count>0)
            {
                step3_decrease_count--;
            }
            else
            {
                step3_decrease_count=10;
                if (step3_wait_time>step3_min_wait_time)
                {
                    step3_wait_time--;
                }
                else{
                    step3_wait_time=step3_min_wait_time;
                }
            }
        }
        }
        else {
            rt_thread_mdelay(10);
        }
}
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
   /* 创建线程控制步进电机1*/
   tid_step = rt_thread_create("t_step_motor",loop_step_motor, RT_NULL, 1024, 0, 10);
  /* 创建成功则启动线程 */
  if (tid_step != RT_NULL)
  {
      rt_thread_startup(tid_step);
  }
  /* 创建线程步进电机2 */
     tid_step1 = rt_thread_create("t_step_motor",loop_step_motor1, RT_NULL, 1024, 0, 10);
    /* 创建成功则启动线程 */
    if (tid_step1 != RT_NULL)
    {
        rt_thread_startup(tid_step1);
    }
    /* 创建线程步进电机3 */
   tid_step2 = rt_thread_create("t_step_motor",loop_step_motor2, RT_NULL, 1024, 0, 10);
  /* 创建成功则启动线程 */
  if (tid_step2 != RT_NULL)
  {
      rt_thread_startup(tid_step2);
  }
   return ret;
}
