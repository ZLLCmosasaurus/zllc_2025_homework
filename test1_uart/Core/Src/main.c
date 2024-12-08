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


#include "esp32.h"

#include "encode.h"

#include "pid.h"

#include "linear velocity.h"

#include "motor.h"

#include "setpwm.h"

#include "omega.h"

#include "gpu.h"
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





usart_1 esp32_instance;

usart_1 *esp32=&esp32_instance;

usart_2 gpu_instance;

usart_2 *gpu=&gpu_instance;

uint8_t receive_data[20];

uint8_t gpu_data[20];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 /* Prevent unused argument(s) compilation warning */
 if(huart->Instance==USART1)
	{
		if(receive_data[0]==0XA5&&receive_data[4]==0X5A)
		{
		Parse_esp32(esp32,receive_data);
		}else{
		HAL_NVIC_SystemReset();
		}
		HAL_UART_Receive_DMA(&huart1,receive_data,5);
	}
   if(huart->Instance==USART2)
	{
		if(gpu_data[0]==0XA5&&gpu_data[3]==0X5A)
		{
			
		Parse_gpu(gpu,gpu_data);
			
		}
		HAL_UART_Receive_DMA(&huart2,gpu_data,4);
	}
 /* NOTE: This function should not be modified, when the callback is needed,
          the HAL_UART_RxCpltCallback could be implemented in the user file
  */
}
uint8_t test_data[5]={0};

int32_t encoder_count[4]={0};

int32_t prev_encoder_count[4]={0};

int32_t delta_count[4];



IncrementalPID pid[4]={
	IncrementalPID(3.7f, 0.118f, 0.01f,-1900.0f, 1900.0f),
	IncrementalPID(2.7f, 1.2f, 0.01f,-1900.0f, 1900.0f),
	IncrementalPID(3.5f, 0.50f, 0.01f,-1900.0f, 1900.0f),
	IncrementalPID(2.0f, 0.1f, 0.05f, -1900.0f, 1900.0f)
};
	
FourWheelDrive car(300, 217, 1900); // 车长，轮距，最大速度，mm

float pwm_lf=0,pwm_rf=0,pwm_lr=0,pwm_rr=0;

float v_measured[4]={0};

float rps[4]={0};

// float steering_angle =PI/2;  //角度

float omega=5.5*((float)esp32_instance.location_x-100.0);

float linear_speed = 0;//;1700*((float)esp32_instance.location_y-100.0);  //线速度mm/s

float v_lf=0, v_rf=0, v_lr=0, v_rr=0; // 前后左右


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) 
			{ 
        for (int i = 0; i < 4; i++)
        {
					
            delta_count[i] = encoder_count[i] - prev_encoder_count[i];
					
            prev_encoder_count[i] = encoder_count[i]; // 更新计数值
					
            // 计算转速 (rps)
						rps[i]= (float)delta_count[i]/ (ENCODER_PPR * TIME_INTERVAL);
					 
					  v_measured[i]=rps[i]*2*PI*34;
					
					// linear_speed = 17.0*(float)esp32_instance.z/20000;
        }
    }
		
		 if (htim->Instance == TIM4){
			 
			 
       //代表0.01s为时间间隔

	if(esp32_instance.UPPER==0){
				omega=5.50*((float)esp32_instance.location_x-100.0)/50;

				linear_speed = 1100.0*((float)esp32_instance.location_y-100.0)/50;  //线速度mm/s

		   
	}else if(esp32_instance.UPPER==1){
				
				omega=gpu_instance.gpu_omega;

		    //上位机角度映射
		
				linear_speed=gpu_instance.gpu_linear;
		  //设初始线速度
	}
				car.computeWheelSpeeds(omega, linear_speed, v_lf, v_rf, v_lr, v_rr);
			 
        pwm_lf= setpwm(pid[0],v_lf,v_measured[0],0,esp32);
     
        pwm_rf= setpwm(pid[1],v_rf,v_measured[1],1,esp32); 
		
        pwm_lr= setpwm(pid[2],v_lr,v_measured[2],2,esp32); 

        pwm_rr= setpwm(pid[3],v_rr,v_measured[3],3,esp32); 
	
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
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_UART_Receive_DMA(&huart1,receive_data,5);
	
	HAL_UART_Receive_DMA(&huart2,gpu_data,4);
	
	HAL_TIM_Base_Start_IT(&htim1);
	
	HAL_TIM_Base_Start_IT(&htim4);
	
	HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
	
	HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_2);
	
	HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_3);
	
	HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_4);
	
	HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_3);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


				
				
	

//		

//				if(esp32_instance.LIFT)//全速前进
//		{
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,100);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,100);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,100);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,100);			
//		}
//		else if(esp32_instance.RIGHT)//右转
//		{
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,50);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,50);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
//		}
//		else if(esp32_instance.LEFT)//左转
//		{
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,50);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,50);
//		}
//	
//		else {
//      if(pwm_lf>=0){

//		int pwm_lf_set=pwm_lf;
//			
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pwm_lf_set);
//			
//    AIN(1);

//		}else{
//			
//		int pwm_lf_set=-pwm_lf;
//			
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pwm_lf_set);

//    AIN(-1);

//		}
//		
//		if(pwm_rf>=0){

//			int pwm_rf_set=-pwm_rf;
//			BIN(1);

//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pwm_rf_set);

//		}else{

//			int pwm_rf_set=-pwm_rf;
//	
//			BIN(-1);
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pwm_rf_set);
//	
//			
//		}
//		
//		if(pwm_lr>=0){
//      
//					int	pwm_lr_set=pwm_lr;

//			CIN(1);
//			
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,pwm_lr_set);
//			
//		}else{
//		
//			int pwm_lr_set=-pwm_lr;
//			
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,pwm_lr_set);

//			CIN(-1);
//		}
//		
//		if(pwm_rr>=0){

//			DIN(1);

//			int pwm_rr_set=pwm_rr;

//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,pwm_rr_set);

//		}else{

//			int pwm_rr_set=-pwm_rr;

//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,pwm_rr_set);

//			DIN(-1);

//		}
//    }

		steering(esp32);

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
