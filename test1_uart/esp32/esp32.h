#ifndef ESP32_H
#define ESP32_H
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef class {
	
	public:
	uint8_t FORWARD;
	uint8_t BACK;
	uint8_t LIFT;
	uint8_t LEFT;
	uint8_t RIGHT;
	uint8_t UPPER;
	uint8_t location_x;
	uint8_t location_y;
	uint32_t z;
	}usart_1;
//esp32数据
	
void Parse_esp32(usart_1 *uart1,uint8_t esp32_data[5]);



#ifdef __cplusplus
}
#endif

#endif
