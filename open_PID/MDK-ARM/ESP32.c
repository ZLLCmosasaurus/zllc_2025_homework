#include<stdio.h>
#include<stdint.h>
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "ESP32.h"
int16_t x_offset[4]={0,0,0,0};
int16_t y_offset[4]={0,0,0,0};
int16_t target_value[4]={0};
ControlData control_data;
int z=4;
void Update_PID_Target(ControlData control_data_xu){
	if(rx_buffer[1]==0x20)
{
	for(int i=0;i<4;i++)
	{// 摇杆值范围：50-150，中心值100
	
	if(control_data.joystick_y>=130||control_data.joystick_y<=70)
	{
		x_offset[i]=0;
		y_offset[i] = (int16_t)(control_data.joystick_y) - 100; // 偏移量
	}
	else if(control_data.joystick_x>=130||control_data.joystick_x<=70)
	{
		y_offset[i]=0;
		x_offset[i] = (int16_t)(control_data.joystick_x) - 100; // 偏移量
	}
	else
	{
		y_offset[i]=(int16_t)(control_data.joystick_y) - 100;  
		x_offset[i]=(int16_t)(control_data.joystick_x) - 100;
	}
}	
	target_value[0]=y_offset[0]- x_offset[0];
	target_value[1]=y_offset[1]+x_offset[1];
	target_value[2]=y_offset[2]-x_offset[2];
	target_value[3]=y_offset[3]+x_offset[3];
}
	if(rx_buffer[1]==0x28)
{
	target_value[0]=0;
	target_value[1]=-49;
	target_value[2]=49;
	target_value[3]=0;	
}
	if(rx_buffer[1]==0x30)
{
	target_value[0]=0;
	target_value[1]=49;
	target_value[2]=-49;
	target_value[3]=0;
}
}
