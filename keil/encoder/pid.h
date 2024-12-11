#ifndef __PID_H__
#define __PID_H__

#include "main.h"
#include "stdio.h"
#include "usart.h"
#include "stdlib.h"
#include "math.h"

typedef struct _pid
{
    float target_val;//目标值
    float actual_val;//实际值
    float err;//定义偏差值
    float err_last;//定义上一个偏差值
    float Kp,Ki,Kd;//定义比例、积分、微分系数
    float integral;//定义积分值
}t_pid;

extern t_pid t_pid_speed_left, t_pid_speed_right;

void PID_Init(void);
void set_pid_target(t_pid *pid,float temp_val);
float get_pid_target(t_pid *pid);
void set_p_i_d(t_pid *pid, float p, float i, float d);
float PID_speed_realize(t_pid *pid,float actual_val);

#endif
