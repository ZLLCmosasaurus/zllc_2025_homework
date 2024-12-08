#include "stdint.h"
#include "usart.h"
#include "Car_Solution.h"
#include "Motor.h"
#include "stm32f1xx_hal.h"

struct SS{
  float Vx;
  float Vy;
  float Vz;
}Raw_Speed;

extern uint8_t NowMode;						//逆天报错，无法include ESP32.h

uint8_t ROS_ReceiveData[4];      //接受上位机的数据
static uint8_t ROS_TranmitData[20] = {0xA5,0xA5,0x02,0x11,0x22,0x33,0x5A};      //将要发送给上位机的数据

//四组16进制强转float
float data_u16_To_Float(uint8_t F1,uint8_t F2,uint8_t F3,uint8_t F4){
  uint32_t hexf = F1<<24 | F2<<16 | F3<<8 | F4;
  float *f;
  f = (float *)(&hexf);

  return *f;
}

//对数据进行解析
 int8_t ROS_DataAnalysis(){
	
	 if(NowMode == 1) return 0;		//保险
	
  if(ROS_ReceiveData[0] != 0xA5 || ROS_ReceiveData[3] != 0x5A){
		
		//HAL_NVIC_SystemReset();
		
    return 0;
  }
   
  Raw_Speed.Vx = (float)(ROS_ReceiveData[1]-128)/100;
  Raw_Speed.Vz = (float)(ROS_ReceiveData[2]-128)/100;

  Move_Transfrom(Raw_Speed.Vx,Raw_Speed.Vz);
	
  return 1;
}

//向上位机发送数据获取请求
void Get_RosData(){
  HAL_UART_Transmit_DMA(&huart3,ROS_TranmitData,7);
}

