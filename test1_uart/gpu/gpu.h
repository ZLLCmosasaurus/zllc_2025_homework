#ifndef GPU_H
#define GPU_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef class 
{
	public:
		
	float gpu_omega;
	
	float gpu_linear;
	
}usart_2;

void Parse_gpu(usart_2* uart1,uint8_t gpu_receive_data[20]);

#ifdef __cplusplus
}
#endif

#endif 

