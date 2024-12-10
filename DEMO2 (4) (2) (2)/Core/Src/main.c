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
#include "motor.h"
#include <math.h>
#include "encode.h"
#include "protocol.h"
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

// 全局变量

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*�������ԣ��ϳ�ʱ�ָ�
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

if (htim->Instance == TIM2) { 
        
        // 计算目标速度（线速度和角速度�???????
       ComputeWheelSpeeds(Vc, Wc, WHEEL_BASE);
          
        for (int i = 0; i < 4; i++) {
            // 当前轮子的实际�?�度
            float current_speed = encoders[i].count * encoder_scale;  // ENCODER_SCALE根据编码器分辨率和减速比计算
            encoders[i].count = 0;  // 清零计数

            float pwm = PID_Compute(&pid[i], target_speed[i], current_speed);// PID计算PWM�???????
            pwm=SET_MOTOR(i, pwm);//切换正反�???
            SetMotorPWM(i, pwm);// 设置PWM
        }
    }
  
}
*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start (&htim4 ,TIM_CHANNEL_1 );
HAL_TIM_PWM_Start (&htim4 ,TIM_CHANNEL_2 );
HAL_TIM_PWM_Start (&htim4 ,TIM_CHANNEL_3 );
HAL_TIM_PWM_Start (&htim4 ,TIM_CHANNEL_4 );
HAL_TIM_Base_Start_IT(&htim2);
HAL_TIM_Base_Start (&htim3); 
DMA_UART_Init();
//HAL_UARTEx_ReceiveToIdle_DMA(&huart2,dma_rx_buffer,128);
    // 初始化PID控制�???????
    for (int i = 0; i < 4; i++) {
        PID_Init(&pid[i], 1.0, 0.1, 0.01);  // 根据�???????要调整PID参数
    };


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    DMA_UART_Process();
// for (int z=0 ; z<15 ;z++){
// Vc=z;
// Wc=z;
// 		       ComputeWheelSpeeds(Vc, Wc, WHEEL_BASE);
          
//         for (int i = 0; i < 4; i++) {
//             // 当前轮子的实际�?�度
//             float current_speed = encoder_count[i] * encoder_scale;  // ENCODER_SCALE根据编码器分辨率和减速比计算
//             encoder_count[i] = 0;  // 清零计数

//             float pwm = PID_Compute(&pid[i], target_speed[i], current_speed);// PID计算PWM�???????
//             SET_MOTOR(i, pwm);//切换正反�???
//             SetMotorPWM(i, pwm);// 设置PWM
// 				}
// 			}

    //DMA_UART_Process_up();
      /* ComputeWheelSpeeds(0.5, 0.2, WHEEL_BASE);
         for (int i = 0; i < 4; i++) 
         {
             // 当前轮子的实际�?�度
             float current_speed = encoder_count[i] * encoder_scale;  // ENCODER_SCALE根据编码器分辨率和减速比计算
             encoder_count[i] = 0;  // 清零计数

             // PID计算PWM�???????
             float pwm = PID_Compute(&pid[i], target_speed[i], current_speed);

             // 设置PWM
             SetMotorPWM(i, pwm);
         }
		*/
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
