#include<stdio.h>
#include<stdint.h>
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "data_transmit.h"
int16_t x_offset[4]={0,0,0,0};
int16_t y_offset[4]={0,0,0,0};
ControlData control_data;
int16_t target_value[4];
//float pid_target_speed_x[4] = {0.0};
//float pid_target_speed_y[4] = {0.0};
void Update_PID_Target(ControlData control_data_xu)
{
for(int i=0;i<4;i++)
{// 摇杆值范围：50-150，中心值100
     x_offset[i] = (int16_t)(control_data.joystick_x) - 100; // 偏移量，因为摇杆初始数据是100，所以减100，判断摇杆向前向后
     y_offset[i] = (int16_t)(control_data.joystick_y) - 100; // 偏移量

    // 根据偏移量设置速度目标值
//   pid_target_speed_x[i] = (float)x_offset[i] * 1; // 比例因子可调整
//    pid_target_speed_y[i] = (float)y_offset[i] * 1;

}	
	target_value[0]=y_offset[0]- x_offset[0];
	target_value[1]=y_offset[1]+x_offset[1];
	target_value[2]=y_offset[2]-x_offset[2];
	target_value[3]=y_offset[3]+x_offset[3];
	


}

