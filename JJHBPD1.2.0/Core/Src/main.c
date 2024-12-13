/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  * 
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include <stdlib.h>
#define ORDER 0x24
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
	/*PID*/
	uint32_t counter_1 = 0;//编码器计数
	float rotations_1 = 0;//转动圈数
	float distance_1 = 0;//行走距离
	int32_t counter_last_1 = 0;//保存上一次编码器计数
	float speed_w_1 = 0;//轮子角速度
	float speed_v_1 = 0;//轮子线速度
	uint8_t direction_2 = 2;//2轮
	uint32_t counter_2 = 0;
	float rotations_2 = 0;
	float distance_2 = 0;
	int32_t counter_last_2 = 0;
	float speed_w_2 = 0;
	float speed_v_2 = 0;
	uint8_t direction_3 = 2;//3轮
	uint32_t counter_3 = 0;
	float rotations_3 = 0;
	float distance_3 = 0;
	int32_t counter_last_3 = 0;
	float speed_w_3 = 0;
	float speed_v_3 = 0;
	uint8_t direction_4 = 2;//4轮
	uint32_t counter_4 = 0;
	float rotations_4 = 0;
	float distance_4 = 0;
	int32_t counter_last_4 = 0;
	float speed_w_4 = 0;
	float speed_v_4 = 0;
	typedef struct 
	{
		float Kp_1,Kp_2,Kp_3,Kp_4;//设置比例系数
		float Ki_1,Ki_2,Ki_3,Ki_4;//设置积分系数
		float Kd_1,Kd_2,Kd_3,Kd_4;//设置微分系数
		float setpoint_1,setpoint_2,setpoint_3,setpoint_4;//目标值
		float integral_1,integral_2,integral_3,integral_4;//误差积分
  float new_error_1,new_error_2,new_error_3,new_error_4;//误差
		float error_1,error_2,error_3,error_4;//上一个误差
  float derivative_1,derivative_2,derivative_3,derivative_4;//微分
		float Output_1,Output_2,Output_3,Output_4; // PID输出
	}PID_TypeDef;
	PID_TypeDef pid;
 // 初始化PID
  void PID_Init_1(PID_TypeDef* pid, float Kp_1, float Ki_1, float Kd_1, float setpoint_1);
  void PID_Init_2(PID_TypeDef* pid, float Kp_2, float Ki_2, float Kd_2, float setpoint_2);
  void PID_Init_3(PID_TypeDef* pid, float Kp_3, float Ki_3, float Kd_3, float setpoint_3);
  void PID_Init_4(PID_TypeDef* pid, float Kp_4, float Ki_4, float Kd_4, float setpoint_4);
  float PID_Compute_moto1(PID_TypeDef*pid);//PID计算函数
  float PID_Compute_moto2(PID_TypeDef*pid);
  float PID_Compute_moto3(PID_TypeDef*pid);
  float PID_Compute_moto4(PID_TypeDef*pid);
  void ControlMotorSpeed_1(float speed);//电机控制函数
  void ControlMotorSpeed_2(float speed);
  void ControlMotorSpeed_3(float speed);
  void ControlMotorSpeed_4(float speed);
  void ControlMotorDirection_stop();//电机方向函数
  void ControlMotorDirection_front();
  void ControlMotorDirection_back();
  void ControlMotorDirection_left_front();
  void ControlMotorDirection_left_back();
  void ControlMotorDirection_left();
  void ControlMotorDirection_right_front();
  void ControlMotorDirection_right_back();
  void ControlMotorDirection_right();


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{ 
 if(huart->Instance==USART2)
 {
   if(receive_data[0]==0xa5&&receive_data[4]==0x5a)
			{
    receive_ESP32_Data.Header=receive_data[0];
				receive_ESP32_Data.button_status=receive_data[1];
				receive_ESP32_Data.joystick_x=hexToDecimal(receive_data[2])-100;
				receive_ESP32_Data.joystick_y=hexToDecimal(receive_data[3])-100;
				receive_ESP32_Data.Tail=receive_data[4];
				if(receive_ESP32_Data.button_status==ORDER)
				{
				 uint8_t PC_Flag[]={0x01,0x10};
				HAL_UART_Transmit_DMA(&huart3,PC_Flag,1);
				}
   }
   else
   {
    HAL_NVIC_SystemReset();
   }
   if(receive_ESP32_Data.joystick_x == 0 && receive_ESP32_Data.joystick_y > 0)//front
   {
      ControlMotorDirection_front();
      PID_Init_1(&pid, 1.0, 0.1, 0.05, receive_ESP32_Data.joystick_y * 8);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, receive_ESP32_Data.joystick_y * 8);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, receive_ESP32_Data.joystick_y * 8);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, receive_ESP32_Data.joystick_y * 8);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
   if(receive_ESP32_Data.joystick_x == 0 && receive_ESP32_Data.joystick_y < 0)//back
   {
      ControlMotorDirection_back();
      PID_Init_1(&pid, 1.0, 0.1, 0.05, -receive_ESP32_Data.joystick_y * 8);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, -receive_ESP32_Data.joystick_y * 8);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, -receive_ESP32_Data.joystick_y * 8);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, -receive_ESP32_Data.joystick_y * 8);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
   if(receive_ESP32_Data.joystick_x < 0 && receive_ESP32_Data.joystick_x > -40 && receive_ESP32_Data.joystick_y > 0)//left front 0-45
   {
      ControlMotorDirection_left_front();
      PID_Init_1(&pid, 1.0, 0.1, 0.05, 400+receive_ESP32_Data.joystick_x * 8);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, 400+receive_ESP32_Data.joystick_x * 8);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, 400);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, 400);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
   if(receive_ESP32_Data.joystick_x <= -40)//left 
   {
      ControlMotorDirection_left();
      PID_Init_1(&pid, 1.0, 0.1, 0.05, 400);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, 400);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, 400);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, 400);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
   if(receive_ESP32_Data.joystick_x < 0 && receive_ESP32_Data.joystick_x > -40 && receive_ESP32_Data.joystick_y < 0)//left back 135-180
   {
      ControlMotorDirection_left_back();
      PID_Init_1(&pid, 1.0, 0.1, 0.05, 400+receive_ESP32_Data.joystick_x * 8);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, 400+receive_ESP32_Data.joystick_x * 8);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, 400);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, 400);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
   if(receive_ESP32_Data.joystick_x > 0 && receive_ESP32_Data.joystick_x < 40 && receive_ESP32_Data.joystick_y > 0)//right front 0-45
   {
      ControlMotorDirection_right_front();
      PID_Init_1(&pid, 1.0, 0.1, 0.05, 400);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, 400);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, 400-receive_ESP32_Data.joystick_x * 8);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, 400-receive_ESP32_Data.joystick_x * 8);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
   if(receive_ESP32_Data.joystick_x > 40)//right 
   {
      ControlMotorDirection_right();
      PID_Init_1(&pid, 1.0, 0.1, 0.05, 400);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, 400);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, 400);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, 400);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
   if(receive_ESP32_Data.joystick_x > 0 && receive_ESP32_Data.joystick_x < 40 && receive_ESP32_Data.joystick_y < 0)//right back
   {
      ControlMotorDirection_right_back();
      PID_Init_1(&pid, 1.0, 0.1, 0.05, 400);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, 400);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, 400-receive_ESP32_Data.joystick_x * 8);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, 400-receive_ESP32_Data.joystick_x * 8);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
   if(receive_ESP32_Data.joystick_x == 0 &&receive_ESP32_Data.joystick_y == 0)//STOP
   {
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_SET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
      PID_Init_1(&pid, 1.0, 0.1, 0.05, 0);//0位要通过xy计算
      PID_Init_2(&pid, 1.0, 0.1, 0.05, 0);
      PID_Init_3(&pid, 1.0, 0.1, 0.05, 0);
      PID_Init_4(&pid, 1.0, 0.1, 0.05, 0);
      ControlMotorSpeed_1(PID_Compute_moto1(&pid));//转到定义改系数
      ControlMotorSpeed_2(PID_Compute_moto2(&pid));
      ControlMotorSpeed_3(PID_Compute_moto3(&pid));
      ControlMotorSpeed_4(PID_Compute_moto4(&pid));
   }
			HAL_UART_Receive_IT(&huart2,receive_data, 5);
   

 }
 
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
if (huart == &huart3)                                                                    
    {
        __HAL_UNLOCK(huart);                                                                
				xUSART_PC.ReceiveNum  = 14;                                                          
        memset(xUSART_PC.ReceiveData, 0, 14);                         
        memcpy(xUSART_PC.ReceiveData, xUSART_PC.BuffTemp, 14);
			for(int i=0;i<12;i++)
			{
			buffer1.char_data[i]=xUSART_PC.ReceiveData[i+1];
			}
     if(xUSART_PC.ReceiveData[0]==0x01)
     {
      HAL_UART_Receive_IT(&huart2, receive_data, 5);
     }
			 if(xUSART_PC.ReceiveData[0]==0x2a&&xUSART_PC.ReceiveData[13]==0x3b)
			{
				receive_PC_Data.Header=xUSART_PC.ReceiveData[0];
				receive_PC_Data.x=buffer1.data[0];
				receive_PC_Data.y=buffer1.data[1];
				receive_PC_Data.yaw=buffer1.data[2];
				receive_PC_Data.Tail=xUSART_PC.ReceiveData[13];
			}
   if (receive_PC_Data.x == 1 && receive_PC_Data.yaw == 0 )
   {
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_SET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,20);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,20);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,20);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,20);
   }
   else if (receive_PC_Data.x == -1 && receive_PC_Data.yaw == 0 ) 
   {
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,20);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,20);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,20);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,20);
   }
   else if (receive_PC_Data.x == 0 && receive_PC_Data.yaw == 1 ) 
   {
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_SET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,40);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,40);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,40);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,40);
   }
   else if (receive_PC_Data.x == 0 && receive_PC_Data.yaw == -1 ) 
   {
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,40);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,40);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,40);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,40);
   }
	 else  
   {
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,0);
      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0);
   }
			 HAL_UARTEx_ReceiveToIdle_DMA(&huart3, xUSART_PC.BuffTemp, 14);
   __HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
		}
}


  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  {
	 if(htim->Instance == TIM2)
	 {
		//计算
		rotations_1 = (float)counter_1 /(1*30*11);//1倍频 30减速比 编码器11PPR
	 distance_1 = rotations_1  * 3.1415926 * 0.065;//轮子直径0.065m
	 speed_w_1 = (float)(counter_1 - counter_last_1)/(1*30*11) * (1000/10) * 60;//uint-rpm
	 speed_v_1 = speed_w_1 / 60 * 3.1415926 * 1;//每秒转速
		counter_last_1 = counter_1 ;//保存上一次编码器读数
		rotations_2 = (float)counter_2 /(1*30*11);
		distance_2 = rotations_2  * 3.1415926 * 0.065;
		speed_w_2 = (float)(counter_2 - counter_last_2)/(1*30*11) * (1000/10) * 60;
		speed_v_2 = speed_w_2 / 60 * 3.1415926 * 1;
		counter_last_2 = counter_2 ;
		rotations_3 = (float)counter_3 /(1*30*11);
		distance_3 = rotations_3  * 3.1415926 * 0.065;
		speed_w_3 = (float)(counter_3 - counter_last_3)/(1*30*11) * (1000/10) * 60;
		speed_v_3 = speed_w_3 / 60 * 3.1415926 * 1;
		counter_last_3 = counter_3 ;
		rotations_4 = (float)counter_4 /(1*30*11);
		distance_4 = rotations_4  * 3.1415926 * 0.065;
		speed_w_4 = (float)(counter_4 - counter_last_4)/(1*30*11) * (1000/10) * 60;
		speed_v_4 = speed_w_4 / 60 * 3.1415926 * 1;
		counter_last_4 = counter_4 ;
	 }
  }
  void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
  {
	 if(GPIO_Pin == GPIO_PIN_8)//电机1A相
	 {
		counter_1 ++;
	 }
	 if(GPIO_Pin == GPIO_PIN_0)//电机2A相
	 { 
		counter_2 ++;
	 }
	 if(GPIO_Pin == GPIO_PIN_6)//电机3A相
	 {
		counter_3 ++;
	 }
	 if(GPIO_Pin == GPIO_PIN_4)//电机4A相
	 {
		counter_4 ++;
	 }
  }
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  //__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,50);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
 // 初始化PID控制器
  PID_Init_1(&pid, 1.0, 0.1, 0.05, 0); // 假设Kp=1.0, Ki=0.1, Kd=0.05, 目标转速0RPM（要调）
  PID_Init_2(&pid, 1.0, 0.1, 0.05, 0);
  PID_Init_3(&pid, 1.0, 0.1, 0.05, 0);
  PID_Init_4(&pid, 1.0, 0.1, 0.05, 0);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
  HAL_TIM_Base_Start_IT(&htim2);
 HAL_UARTEx_ReceiveToIdle_DMA(&huart3, xUSART_PC.BuffTemp, 14);
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
 HAL_UART_Receive_IT(&huart2,receive_data, 5);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 
  {   
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void PID_Init_1(PID_TypeDef* pid, float Kp_1, float Ki_1, float Kd_1, float setpoint_1) {
    pid->Kp_1 = Kp_1;
    pid->Ki_1 = Ki_1;
    pid->Kd_1  = Kd_1 ;
    pid->setpoint_1 = setpoint_1;
    pid->integral_1 = 0.0f;
    pid->error_1 = 0.0f;
    pid->Output_1 = 0.0f;
}
void PID_Init_2(PID_TypeDef* pid, float Kp_2, float Ki_2, float Kd_2, float setpoint_2) {
    pid->Kp_2 = Kp_2;
    pid->Ki_2 = Ki_2;
    pid->Kd_2  = Kd_2;
    pid->setpoint_2 = setpoint_2;
    pid->integral_2 = 0.0f;
    pid->error_2 = 0.0f;
    pid->Output_2 = 0.0f;
}
void PID_Init_3(PID_TypeDef* pid, float Kp_3, float Ki_3, float Kd_3, float setpoint_3) {
    pid->Kp_3 = Kp_3;
    pid->Ki_3 = Ki_3;
    pid->Kd_3 = Kd_3;
    pid->setpoint_3 = setpoint_3;
    pid->integral_3 = 0.0f;
    pid->error_3 = 0.0f;	
    pid->Output_3 = 0.0f;
}
void PID_Init_4(PID_TypeDef* pid, float Kp_4, float Ki_4, float Kd_4, float setpoint_4) {
    pid->Kp_4 = Kp_4;
    pid->Ki_4 = Ki_4;
    pid->Kd_4  = Kd_4 ;
    pid->setpoint_4 = setpoint_4;
    pid->integral_4 = 0.0f;
    pid->error_4 = 0.0f;
    pid->Output_4 = 0.0f;
}
float PID_Compute_moto1(PID_TypeDef*pid)
{
	   pid->new_error_1 = pid->setpoint_1 - speed_w_1;  // 计算误差
    pid->integral_1 += pid->new_error_1;  // 积分
    pid->derivative_1 = pid->new_error_1 - pid->error_1;  // 计算微分
    pid->Output_1 = pid->Kp_1 * pid->new_error_1 + pid->Ki_1 * pid->integral_1 + pid->Kd_1 * pid->derivative_1;// 计算PID输出
    pid->error_1 = pid->new_error_1;  // 更新上一个误差
    return pid->Output_1;  // 返回控制输出
}
float PID_Compute_moto2(PID_TypeDef*pid)
{
	   pid->new_error_2 = pid->setpoint_2 - speed_w_2;  // 计算误差
    pid->integral_2 += pid->new_error_2;  // 积分
    pid->derivative_2 = pid->new_error_2 - pid->error_2;  // 计算微分
    pid->Output_2 = pid->Kp_2 * pid->new_error_2 + pid->Ki_2 * pid->integral_2 + pid->Kd_2 * pid->derivative_2;// 计算PID输出
    pid->error_2 = pid->new_error_2;  // 更新上一个误差
    return pid->Output_2;  // 返回控制输出
}
float PID_Compute_moto3(PID_TypeDef*pid)
{
	   pid->new_error_3 = pid->setpoint_3 - speed_w_3;  // 计算误差
    pid->integral_3 += pid->new_error_3;  // 积分
    pid->derivative_3 = pid->new_error_3 - pid->error_3;  // 计算微分
    pid->Output_3 = pid->Kp_3 * pid->new_error_3 + pid->Ki_3 * pid->integral_3 + pid->Kd_3 * pid->derivative_3;// 计算PID输出
    pid->error_3 = pid->new_error_3;  // 更新上一个误差
    return pid->Output_3;  // 返回控制输出
}
float PID_Compute_moto4(PID_TypeDef*pid)
{
	   pid->new_error_4 = pid->setpoint_4 - speed_w_4 ;  // 计算误差
    pid->integral_4 += pid->new_error_4;  // 积分
    pid->derivative_4 = pid->new_error_4 - pid->error_4;  // 计算微分
    pid->Output_4 = pid->Kp_4 * pid->new_error_4 + pid->Ki_4 * pid->integral_4 + pid->Kd_4 * pid->derivative_4;// 计算PID输出
    pid->error_4 = pid->new_error_4;  // 更新上一个误差
    return pid->Output_4;  // 返回控制输出
}
void ControlMotorSpeed_1(float speed)
{
    __HAL_TIM_SET_COMPARE (&htim3 ,TIM_CHANNEL_3,(uint16_t)speed );//具体什么系数要调
}
void ControlMotorSpeed_2(float speed)
{
    __HAL_TIM_SET_COMPARE (&htim3 ,TIM_CHANNEL_4,(uint16_t)speed );//具体什么系数要调
}
void ControlMotorSpeed_3(float speed)
{
    __HAL_TIM_SET_COMPARE (&htim4 ,TIM_CHANNEL_3,(uint16_t)speed );//具体什么系数要调
}
void ControlMotorSpeed_4(float speed)
{
    __HAL_TIM_SET_COMPARE (&htim4 ,TIM_CHANNEL_4,(uint16_t)speed );//具体什么系数要调
}
void ControlMotorDirection_stop()
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}
void ControlMotorDirection_front()
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_SET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}
void ControlMotorDirection_back()
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
}
void ControlMotorDirection_right_front()
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_SET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}

void ControlMotorDirection_right_back()
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
}
void ControlMotorDirection_left_front()//0-45度
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_SET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}
void ControlMotorDirection_left_back()
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
}
void ControlMotorDirection_left()
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_SET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}
void ControlMotorDirection_right()
{
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//电机1
      HAL_GPIO_WritePin (GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//电机2
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);//电机3
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//电机4
      HAL_GPIO_WritePin (GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
