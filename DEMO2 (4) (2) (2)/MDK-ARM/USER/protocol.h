#ifndef PROTOCOL_H
#define PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f1xx_hal.h"
// 数据包定义
#define FRAME_HEADER   0xA5
#define FRAME_TAIL     0x5A
#define BUFFER_SIZE    20
#define PACKET_SIZE    5
#define JOYSTICK_MID   100
#define JOYSTICK_MAX   200
#define JOYSTICK_MIN   0

extern float linear_v;
extern float angular_v;

// 数据包结构
typedef struct {
    uint8_t FORWARD;
	uint8_t BACK;
	uint8_t JUMP;
	uint8_t LEFT;
	uint8_t RIFGT;
	uint8_t ROBOT_GO;
    uint8_t buttonState;
    uint8_t joystickX;
    uint8_t joystickY;
} ControlData;

typedef struct {
    uint8_t W;
	uint8_t V;

} ControlData_up;
// 函数声明
void Process_ControlData(ControlData *data);
void DMA_UART_Init(void);
void DMA_UART_Process(void);
void DMA_UART_Process_up(void);
void Process_ControlData_up(ControlData *data);
#ifdef __cplusplus
}
#endif
 
#endif // PROTOCOL_H


//extern  uint8_t dma_rx_buffer[];
	// typedef class {
	// public:
	// uint8_t FORWARD;
	// uint8_t BACK;
	// uint8_t JUMP;
	// uint8_t LEFT;
	// uint8_t RIFGT;
	// uint8_t ROBOT_GO;
	// uint8_t location_x;
	// uint8_t location_y;
	// uint8_t z;
	// }usart_1;
//定义类储存USART1传输数据
// void Parse_esp32(usart_1 *uart1,uint8_t esp32_data[5]);
