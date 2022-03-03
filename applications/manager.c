/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     DUAN       the first version
 */
#include "manager.h"

static const int  draw_time = 10 ; //抽水时间
//全局变量
u_int relay_status=1; //定时器状态
u_int row = 50;// 横向摇杆接受值0-99
u_int col = 50;// 纵向摇杆接受值0-99
u_int led_status = 0;    //   当前状态灯编码  0 绿色  1 黄色  2 红色  3 蜂鸣
u_int current_theta=0; //当前角 z轴角度
float_t dump_energy = 0;  //  当前剩余电量
float_t back_home_erergy = 15;  //  返航电量阈值
int current_status=1;  // 船当前状态  1  运行（绿）  2 返航（红） 3 抽水（黄）
double lng=0.0;  // 经度
double lat=0.0;   // 纬度
double target_longitude=0;  // 目标经度
double target_latitude=0;   // 目标纬度
int b_back_home=0;// 是否在返航
bool b_limit_switch_1=true;  // 限位开关1  输入上拉 公共端接地
bool b_limit_switch_2=true; // 限位开关2
bool b_limit_switch_3=true; // 限位开关3
bool b_limit_switch_4=true;  // 限位开关4
double angle_x=0.1;//  x轴角度
double angle_y=0.2;//  y轴角度
double deep=0.0;  // 深度
int tem=0;  // 温度
// 接收上位机指令
/*
 * S1,2Z
 * P1500,1500Z
 * */
int step_motor[2]={3,3}; //步进电机运动指令 1 正转  2 反转  3 停止
int pwm_motor[2]={1500,1500}; //主侧电机运动指令
int line_step_motor=3;   //拉线步进电机 1 正转放线  2 反转收线  3 停止
int relay1=2;   //继电器数据  1开  2关
static rt_thread_t t_manager = RT_NULL;

void manager(void *parameter)
{
    double last_angle_x=0.0;//  x轴角度
    double last_angle_y=0.0;//  y轴角度
    u_int last_current_theta=0; //当前角 z轴角度
    double last_lng=0.1;  // 经度
    double last_lat=0.0;   // 纬度
    double last_deep=0.2;   //深度
    bool last_b_limit_switch_1=true;  // 限位开关1
    bool last_b_limit_switch_2=true; // 限位开关2
    bool last_b_limit_switch_3=true; // 限位开关3
    bool last_b_limit_switch_4=true;  // 限位开关4
    while(1)
    {
        // 发送数据到上位机
        // 判断数据是否更新，更新了就发送
        if (last_b_limit_switch_1!=b_limit_switch_1||last_b_limit_switch_2!=b_limit_switch_2||last_b_limit_switch_3!=b_limit_switch_3||last_b_limit_switch_4!=b_limit_switch_4)
        {
            char limit_buf[10];
            sprintf(limit_buf, "L%d,%d,%d,%dZ\n",b_limit_switch_1,b_limit_switch_2,b_limit_switch_3,b_limit_switch_4);
            printf("limit buf %s\n",limit_buf);
            last_b_limit_switch_1=b_limit_switch_1;
            last_b_limit_switch_2=b_limit_switch_2;
            last_b_limit_switch_3=b_limit_switch_3;
            last_b_limit_switch_4=b_limit_switch_4;
            send_data(limit_buf,10);
            rt_thread_mdelay(10);
        }
        if (last_deep!=deep)
        {
            char deep_buf[10];
            sprintf(deep_buf, "D%.2f,%dZ\r\n",deep,tem);
            printf("deep_buf %s\n",deep_buf);
            last_deep=deep;
            send_data(deep_buf,10);
            rt_thread_mdelay(10);
        }
        if (last_angle_x!=angle_x||last_angle_y!=angle_y||last_current_theta!=current_theta||last_lng!=lng||last_lat!=lat)
        {
            char pos_buf[50];
            sprintf(pos_buf, "P%d,%d,%dZ\n",(int)angle_y,(int)angle_x,current_theta);
//            printf("pos buf %s\n",pos_buf);
            last_angle_x=angle_x;
            last_angle_y=angle_y;
            last_current_theta=current_theta;
            last_lng=lng;
            last_lat=lat;
            send_data(pos_buf,50);
            rt_thread_mdelay(10);
        }
        control_led(relay1);
        set_pwm(pwm_motor[0], pwm_motor[1]);   // 控制无刷电机
//        printf("step_motor[0] :%d,step_motor[1] :%d\n",step_motor[0],step_motor[1]);
//        printf("pwm_motor[0] :%d,pwm_motor[1] :%d\n",pwm_motor[0],pwm_motor[1]);
//        printf("line_step_motor:%d\n",line_step_motor);
        rt_thread_mdelay(50);
    }
}



/* 线程 */
int thread_manager(void)
{
    /* 创建线程 */
    t_manager = rt_thread_create("thread_manager",
                                  manager,
                                  RT_NULL,
                                  2048,
                                  20,
                                  5);

    /* 如果获得线程控制块，启动这个线程 */
    if (t_manager != RT_NULL)
        rt_thread_startup(t_manager);
    return 0;
}
