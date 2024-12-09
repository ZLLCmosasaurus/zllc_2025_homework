#include<stdio.h>
#include<stdint.h>
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "PID.h"
PID motor_pid[4] = {
    {1.0, 0.1, 0.05, 0, 0},
    {1.0, 0.1, 0.05, 0, 0},
    {1.0, 0.1, 0.05, 0, 0},
    {1.0, 0.1, 0.05, 0, 0}//通过控制系数进而进行调试
};
int16_t PID_Compute(PID* pid, int16_t target_value, int16_t actual_value) {
     float error = target_value - actual_value;
    pid->integral += error;
    float derivative = error - pid->prev_error;
    pid->prev_error = error;
    return (int16_t)(pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative);
	//这个函数返回输出给speed，speed就是set_compare的比较寄存器的值，通过它改变输出
}

