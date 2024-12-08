/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M1_Toward_Pin GPIO_PIN_5
#define M1_Toward_GPIO_Port GPIOA
#define M1_PWM_Pin GPIO_PIN_6
#define M1_PWM_GPIO_Port GPIOA
#define M2_PWM_Pin GPIO_PIN_7
#define M2_PWM_GPIO_Port GPIOA
#define M3_PWM_Pin GPIO_PIN_0
#define M3_PWM_GPIO_Port GPIOB
#define M4_PWM_Pin GPIO_PIN_1
#define M4_PWM_GPIO_Port GPIOB
#define IN0_Pin GPIO_PIN_12
#define IN0_GPIO_Port GPIOB
#define IN1_Pin GPIO_PIN_13
#define IN1_GPIO_Port GPIOB
#define M4_Toward_Pin GPIO_PIN_14
#define M4_Toward_GPIO_Port GPIOB
#define M3_Toward_Pin GPIO_PIN_15
#define M3_Toward_GPIO_Port GPIOB
#define SERVO2_Pin GPIO_PIN_8
#define SERVO2_GPIO_Port GPIOA
#define SERVO3_Pin GPIO_PIN_9
#define SERVO3_GPIO_Port GPIOA
#define SERVO1_Pin GPIO_PIN_11
#define SERVO1_GPIO_Port GPIOA
#define M2_Toward_Pin GPIO_PIN_12
#define M2_Toward_GPIO_Port GPIOA
#define EncoderA_Pin GPIO_PIN_6
#define EncoderA_GPIO_Port GPIOB
#define EncoderB_Pin GPIO_PIN_7
#define EncoderB_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
