#include "esp32.h"

void Parse_esp32(usart_1 *uart1,uint8_t esp32_data[5])
	{
	
	uart1->FORWARD=(esp32_data[1]&0x01);
	
	uart1->BACK=(esp32_data[1]&0x02)>>1;
	
	uart1->LIFT=(esp32_data[1]&0x04)>>2;
	
	uart1->LEFT=(esp32_data[1]&0x08)>>3;
	
	uart1->RIGHT=(esp32_data[1]&0x10)>>4;
	
	uart1->UPPER=(esp32_data[1]&0x20)>>5;
	
	uart1->location_x=esp32_data[2];
	
	uart1->location_y=esp32_data[3];
	
//	int x=uart1->location_x;

//	int y=uart1->location_y;

	//uart1->z=(x-100)*(x-100)+(y-100)*(y-100);
	
	}//

