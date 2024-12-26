/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
struct xUART_TypeDef_PC
{
uint16_t ReceiveNum;	
uint8_t ReceiveData[14];     
uint8_t BuffTemp[14]; 
}__attribute__((packed));

struct xUART_TypeDef_PC xUSART_PC={0};

struct Receive_PC_Data
{
	uint8_t Header;
 float x;
 float y;
 float yaw;
	uint8_t Tail;
} __attribute__((packed));

struct Receive_ESP32_Data
{
	uint8_t Header;
 uint8_t button_status;
 int joystick_x;
 int joystick_y;
	uint8_t Tail;
} __attribute__((packed));


 union Buffer{
	float data[3];	
	char char_data[12];
}buffer1;
 
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern struct xUART_TypeDef_PC xUSART_PC;
extern struct Receive_PC_Data receive_PC_Data;
extern struct Receive_ESP32_Data receive_ESP32_Data;
extern uint8_t receive_data[5];
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
int hexToDecimal(uint8_t hexValue);

/* USER CODE END Priva;te defines */

void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
