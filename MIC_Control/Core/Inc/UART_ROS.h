#ifndef __UART_ROS_H
#define __UART_ROS_H


extern uint8_t ROS_ReceiveData[100];
extern uint8_t ROS_TranmitData[100];
extern struct SS Raw_Speed;

#ifdef __cplusplus
extern "C" {
#endif

int8_t ROS_DataAnalysis();
void Get_RosData();

#ifdef __cplusplus
}
#endif

#endif