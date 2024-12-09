#include<stdio.h>
#include<stdint.h>
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "ROS.h"
int16_t actual_ros_value[2];
ControlRosData controlrosdata;
int16_t target_ros_speed[4];
void Update_ROS_Target(ControlRosData controlrosdata_xu)
{
	actual_ros_value[0]=controlrosdata.ros_line-128;
	actual_ros_value[1]=controlrosdata.ros_yaw-128;
	if(actual_ros_value[0]!=0&&actual_ros_value[1]!=0)
	{
		target_ros_speed[0]=0;
		target_ros_speed[1]=0;
		target_ros_speed[2]=0;
		target_ros_speed[3]=0;
	}
	else if(actual_ros_value[0]!=0&&actual_ros_value[1]==0) 
	{
		target_ros_speed[0]=actual_ros_value[0];
		target_ros_speed[1]=actual_ros_value[0];
		target_ros_speed[2]=actual_ros_value[0];
		target_ros_speed[3]=actual_ros_value[0];
	}
	else if(actual_ros_value[0]==0&&actual_ros_value[1]!=0)
	{
		target_ros_speed[0]=0;
		target_ros_speed[1]=actual_ros_value[1];
		target_ros_speed[2]=-actual_ros_value[1];
		target_ros_speed[3]=0;
	}
	else
	{
		target_ros_speed[0]=0;
		target_ros_speed[1]=0;
		target_ros_speed[2]=0;
		target_ros_speed[3]=0;
		
	}
	
}

