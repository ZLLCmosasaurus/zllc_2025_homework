#ifndef WF_DATA_H
#define WF_DATA_H

#include "main.h"
#include "Motor.h"
#include "Move.h"

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart2;
extern uint8_t data_buffer[5];
extern uint8_t data_index ;
extern uint8_t rx_buffer[20];
extern uint8_t rx_byte ;
extern uint8_t frame_received;
extern uint16_t write_index ;  // 缓冲区写入索引
extern uint16_t read_index ;   // 缓冲区读取索引
extern uint8_t receiveData[2];
void WF_ControlCar(uint8_t data[5], int speed[4]);
void ProcessReceivedData(void);
#endif // WF_DATA_H
