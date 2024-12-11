#include "pid.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"

t_pid t_pid_speed_left, t_pid_speed_right;

void PID_Init()
{
   /* 左边电机速度初始化参数 */
    t_pid_speed_left.target_val=0.0;
    t_pid_speed_left.actual_val=0.0;
    t_pid_speed_left.err=0.0;
    t_pid_speed_left.err_last=0.0;
    t_pid_speed_left.integral=0.0;

    t_pid_speed_left.Kp = 1.1;
    t_pid_speed_left.Ki = 0.68;
    t_pid_speed_left.Kd = 0.068;

    /* 右电机速度相关初始化参数 */
    t_pid_speed_right.target_val=0.0;
    t_pid_speed_right.actual_val=0.0;
    t_pid_speed_right.err=0.0;
    t_pid_speed_right.err_last=0.0;
    t_pid_speed_right.integral=0.0;

    t_pid_speed_right.Kp = 0.965;
    t_pid_speed_right.Ki = 0.810;
    t_pid_speed_right.Kd = 0.667;
}

void set_pid_target(t_pid *pid,float temp_val)
{
    pid->target_val = temp_val;// 设置当前的目标值
}

float get_pid_target(t_pid *pid)
{
  return pid->target_val;// 获取当前的目标值
}

void set_p_i_d(t_pid *pid,float p, float i, float d)
{
    pid->Kp = p;// 设置比例系数 P
    pid->Ki = i;// 设置积分系数 I
    pid->Kd = d;// 设置微分系数 D
}

float PID_speed_realize(t_pid *pid,float actual_val)
{
    pid->err = pid->target_val - actual_val;//计算目标值与实际值之间的误差
    if((pid->err<0.3)&&(pid->err>-0.3))pid->err=0;//pid死区，当误差值很小时可以忽略不计
    pid->integral += pid->err;//误差积累

    //积分限幅，1000值可调
    if(pid->integral>1000)pid->integral=1000;
    else if(pid->integral<-1000)pid->integral=-1000;

    pid->actual_val = pid->Kp * pid->err + pid->Ki * pid->integral + pid->Kd * (pid->err - pid->err_last);//位置式PID算法公式
    pid->err_last = pid->err;//误差传递
    return pid->actual_val;//返回能实现达到目标值的需求速度值
}







