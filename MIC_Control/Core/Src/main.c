//本版本为加时赛后完善了一下的代码
//30.4    测速带一阶卡尔曼滤波的版本


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
#include "ESP32.h"
#include "Motor.h"
#include "UART_ROS.h"
#include "Car_Solution.h"
#include "Kalman.h"

//test
#include <stdio.h>
#include <string.h>

//char TestSpeed[20];
//test

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t flag_TIM = 0; // 记录定时器现在是否空闲
int8_t Count_Motor;

// 重写HAL_InTick函数，用于ROS信息的定时请求
uint8_t Tick = 0, flag_Ros = 0, flag_ESP = 0;				//flag_Ros 1 接收错误 ， 0 正确
uint8_t Ros_Receive_Error;


void HAL_IncTick(void)
{

  uwTick += uwTickFreq;
  Tick++;
	
	//利用滴答定时器，实现数据错误时，延时一段时间后重新接收
  if (Tick == 1)
  {
		if(flag_Ros == 1){						
			flag_Ros = 0;
			HAL_UART_Receive_DMA(&huart3,ROS_ReceiveData,4);
		}
		if(flag_ESP == 1){
			flag_ESP = 1;
			HAL_UART_Receive_DMA(&huart2, ESP32_Data, 5);
		}
    Tick = 0;	
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // 进入定时器测速
  if (htim == &htim2)
  {
    HAL_TIM_Base_Stop_IT(&htim2);
    Count_Motor = __HAL_TIM_GetCounter(&htim4);

    if (M0 == &M2 || M0 == &M3)
      Count_Motor = -Count_Motor;
		
		
    //M0->flag  = Count_Motor > 0 ? 1 : 0;      已经删了flag
    //M0->Last_Speed = M0->Speed;
    //M0->Speed = Count_Motor;                      // 记录的是10ms内的脉冲数
    //M0->Speed = (M0->Speed + M0->Last_Speed) / 2; // 采用均值滤波，防止应定时器开启时机造成的测速偏差
//		
//		M0->Speed[0] = M0->Speed[1];
//		M0->Speed[1] = M0->Speed[2];
//		M0->Speed[2] = M0->Speed[3];
//		M0->Speed[3] = Count_Motor;
//		//4作为临界，小于4的转速是无法直接均值滤波出来的
//		if(M0->Speed[3] > 4){
//			M0->Speed[3] = (M0->Speed[0] + M0->Speed[1] + M0->Speed[2] + M0->Speed[3])/4;
//		}
		M0->Speed[3] = Count_Motor;
		
		M0->Speed[3] = Kalman_fifter(M0->Speed[3]);
		
		//M0->Speed[3] = Count_Motor;
		
//		if(M0 == &M1){
//			PI_Motor(M0);   
//		}
    PID_Motor(M0);
		
		Set_Speed(M0,M0->PWM);
	
		//电机对应，只测1 2电机，1--4，2--3
		if(M0 == &M1){
			Set_Speed(&M4,M0->PWM);
		}
		else if(M0 == &M2){
			Set_Speed(&M3,M0->PWM);
		}
		
		flag_TIM = 0;
		
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // ESP32遥感数据
  if (huart == &huart2)
  {
    if (ESP32_Data[0] != 0xA5 && ESP32_Data[4] != 0x5A)
    {
      //HAL_NVIC_SystemReset(); // 第一帧接收数据帧错误时重启，重启，实现缓冲区清空缓冲区，可能会引起问题
															// 方法二，写一个数据缓冲区，有点麻烦    错误，不可行！！,应该不是写软件缓冲区可以解决的
			//HAL_UART_ClearReceiveBuffer(&huart2);
			flag_ESP = 1;
    }
    else
    {
			flag_ESP = 0;
			ESP32_Analysis();
      HAL_UART_Receive_DMA(&huart2, ESP32_Data, 5); //UART自带缓冲区，DMA与IT效果一样，若CPU占用高，对缓冲区内数据处理慢，则会覆盖
																										//以下两行注释错误
																										// 中断接受ESP32数据，不要用DMA，连续多次发送会造成buff覆盖
                                                    // 使用中断模式，则会一直接收到5个后，才释放串口状态，再开启，我猜
    }
  }
  // 上位机数据
  if (huart == &huart3 && NowMode == 0)
  {
		if(ROS_ReceiveData[0] != 0xA5 || ROS_ReceiveData[3] != 0x5A){
			flag_Ros = 1;
			
		}
		else{
			flag_Ros = 0;
			ROS_DataAnalysis();
			HAL_UART_Receive_DMA(&huart3, ROS_ReceiveData, 4);
		}
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_DMA(&huart2, ESP32_Data, 5); // 开启与ESP32的通信接收

  Struct_Init(); // 对电机结构体状态的初始化
  Motor_Init();         // 开启电机四路PWM
  Servo_Init();         // 初始化舵机，开启PWM

  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // 编码器测速通道开启
	
	/*测试方便用
  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);
  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 0);
	*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//flag_Mode
		if(NowMode == 1){
			
		}
		
    if (flag_TIM == 0 && NowMode == 0)
		//if (flag_TIM == 0)
    {
      flag_TIM = 1; // 定时器被占用
			
      if (Now_Motor == 1)
      {
        Now_Motor++;
        // 444模拟开关切换
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
				HAL_Delay(2);
        Get_Speed(&M1);
				
      }
      else if (Now_Motor == 2)
      { 
        Now_Motor = 1;
        // 444模拟开关切换
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
				HAL_Delay(2);
        Get_Speed(&M2);
      }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//PID_SET   test记得删
		//PID_SET(Kp,Ki,Kd);
		//
  /* USER CODE END 3 */
	}
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
