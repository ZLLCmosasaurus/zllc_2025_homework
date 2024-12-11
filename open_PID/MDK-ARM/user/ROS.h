#ifndef ROS_H
#define ROS_H
typedef struct{
	uint8_t ros_line;
	uint8_t ros_yaw;
}ControlRosData;
extern ControlRosData controlrosdata;
extern int16_t target_ros_speed[4];
void Update_ROS_Target(ControlRosData controlrosdata_xu);
#endif
