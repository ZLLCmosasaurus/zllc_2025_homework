#include "gpu.h"

void Parse_gpu(usart_2* uart1,uint8_t gpu_receive_data[4])
{
	
	
	uart1->gpu_omega=(static_cast<float>(gpu_receive_data[1])/255)*14-7;
	uart1->gpu_linear=(static_cast<float>(gpu_receive_data[2]))*2;
}
