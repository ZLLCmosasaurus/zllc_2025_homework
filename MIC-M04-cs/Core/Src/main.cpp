/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pid.h"
#include "Motor.h"
#include "Move.h"
#include "WF_data.h"
#include "HC_data.h"
#include "string.h"
#include "math.h"
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

// WF 数据接受相关变量
uint8_t rx_byte;
uint8_t rx_buffer[BUFFER_SIZE]={0};
uint8_t data_buffer[DATA_LEN] = {0};
uint8_t WF_data[DATA_LEN];
uint8_t data_index = 0;
uint8_t frame_received = 0;
uint16_t write_index = 0;
uint16_t read_index = 0;

//上位机通讯相关变量
uint8_t receiveData[2]={0x55,0x44};

// 编码器相关变量
int32_t encoder_count[4] = {0};
int32_t delta_count[4] = {0};
float speed_rps[4] = {0.0f};
float speed_rpm[4] = {0.0f};

// PID相关变量与参数
float iP = 2.4, iI = 0.0, iD = 0.0, oP = 0.1, oI = 0.0, oD = 0.0;
PID pid(iP, iI, iD, 5, 100);
PIDCascade pid_Cascade(oP, oI, oD, 20, 100, iP, iI, iD, 20, 100);

// 速度变量
float speed[4] = {0};
float pid_output[4] = {0.0f, 0.0f, 0.0f, 0.0f};
int target_speed[4]={25,25,25,25};
int pwm_output[4] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	// 初始化定时器3
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);	    // TIM1_CH1(pwm)
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);	    // TIM1_CH1(pwm)
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);	    // TIM1_CH1(pwm)
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);	    // TIM1_CH1(pwm)
	//初始化定时器2
  HAL_TIM_Base_Start_IT(&htim2);                // 使能定时器2中断
    // 初始化串口3
  HAL_UART_Receive_IT(&huart3, &rx_byte, 1);
	HAL_UART_Transmit_IT(&huart2, receiveData, 2);
	HAL_UART_Receive_IT(&huart2, receiveData, 2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  #define OPEN//TEST or GO or OPEN
  while (1)
  {
    #ifdef TEST
    // pwm_output 通过 PID 计算得出，范围为 0 到 100
	  for (int i = 0; i < 4; i++)
	      	{
	          pid_output[i] = pid.calculate(target_speed[i], abs(speed[i]));
	          // 限制 PWM 输出范围
	          pwm_output[i]=int(pid_output[i]);
	      	}
    // 设置电机速度
    Move_Front(pwm_output);
    #endif


    #ifdef GO
        //处理接收数据
    ProcessReceivedData();
    // 主循环中检查是否有完整的数据帧
    if (frame_received)
    {
      memcpy(WF_data, data_buffer, DATA_LEN);
      frame_received = 0; // 清除接收标志
        for (int i = 0; i < 4; i++)
    	{
        pid_output[i] = pid.calculate(target_speed[i], abs(speed[i]));
        // 限制 PWM 输出范围
        pwm_output[i]=int(pid_output[i]);
    	}
      WF_ControlCar(WF_data, pwm_output);
    }
  #endif
		#ifdef OPEN
		        //处理接收数据
    ProcessReceivedData();
    // 主循环中检查是否有完整的数据帧
    if (frame_received)
    {
      memcpy(WF_data, data_buffer, DATA_LEN);
      frame_received = 0; // 清除接收标志
        for (int i = 0; i < 4; i++)
    	{
        // 限制 PWM 输出范围
        pwm_output[i]=80;
    	}
      WF_ControlCar(WF_data, pwm_output);
    }
		
		#endif
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
