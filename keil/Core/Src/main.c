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
#include "stdio.h"
#include "encoder.h"
#include "pid.h"
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
double motor1_pulse=0;
double motor2_pulse=0;
uint8_t receivedBuffer[5];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct 
{
  uint8_t frameHeader;
  uint8_t keyState;
  uint8_t joystickX;
  uint8_t joystickY;
  uint8_t frameTail;
}RobotControlPacket;



void motor_direction_forward()
{
    motorControlL(1);
    motorControlR(1);
}

void motor_direction_back()
{
    motorControlL(2);
    motorControlR(2);
}

void motor_direction_left()
{
    motorControlL(2);
    motorControlR(1);
}

void motor_direction_right()
{
    motorControlL(1);
    motorControlR(2);
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
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
encoder_Init();
PID_Init();
//HAL_UART_Receive_DMA(&huart3,receivedBuffer,5);
HAL_UART_Receive_DMA(&huart3,receivedBuffer,5);
//HAL_UART_Receive_IT(&huart3,receivedBuffer,5);
RobotControlPacket receivedPacket;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      if(receivedPacket.frameHeader==0xA5&&receivedPacket.frameTail==0x5A)
      {
        switch (receivedPacket.keyState)
        {
         case 0x01:{
          motor_direction_forward();
				  PWMA(800);
				  PWMB(800);
          motor_direction_forward();
          motorL_Target_speed(motor1_pulse,800);
          motorR_Target_speed(motor2_pulse,800);
          PWMB(speed_left_pid_pwm(motor1_pulse));
          PWMA(speed_right_pid_pwm(motor2_pulse));
         }
          break;
         case 0x02:{
          motor_direction_back();
				  PWMA(800);
				  PWMB(800);
          motorL_Target_speed(motor1_pulse,800);
          motorR_Target_speed(motor2_pulse,800);
          PWMB(speed_left_pid_pwm(motor1_pulse));
          PWMA(speed_right_pid_pwm(motor2_pulse));
         }
          break;
         case 0x04:{
          motor_direction_left();
				  PWMA(300);
				  PWMB(300);
          motorL_Target_speed(motor1_pulse,300);
          motorR_Target_speed(motor2_pulse,300);
          PWMB(speed_left_pid_pwm(motor1_pulse));
          PWMA(speed_right_pid_pwm(motor2_pulse));
         }
          break;
         case 0x10:{
          motor_direction_right();
				  PWMA(300);
				  PWMB(300);
          motorL_Target_speed(motor1_pulse,300);
          motorR_Target_speed(motor2_pulse,300);
          PWMB(speed_left_pid_pwm(motor1_pulse));
          PWMA(speed_right_pid_pwm(motor2_pulse));
         }
          break;
         default:
          break;
        }
      }

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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//外部中断定时读取编码器脉冲数
{
  if(GPIO_Pin==GPIO_PIN_0)
  {
    motor1_pulse++;
  }
  if(GPIO_Pin==GPIO_PIN_1)
  {
    motor1_pulse++;
  }
  if(GPIO_Pin==GPIO_PIN_2)
  {
    motor2_pulse++;
  }
  if(GPIO_Pin==GPIO_PIN_7)
  {
    motor2_pulse++;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM2)
  {
    Get_motorL_Encoder(motor1_pulse);
    Get_motorR_Encoder(motor2_pulse);
    speed_left_pid_control(motor1_pulse);
    speed_right_pid_control(motor2_pulse);
    speed_left_pid_pwm(motor1_pulse);
    speed_right_pid_pwm(motor2_pulse);
    PWMB(speed_left_pid_pwm(motor1_pulse));
    PWMA(speed_right_pid_pwm(motor2_pulse));
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	
HAL_UART_Receive_DMA(&huart3,receivedBuffer,5);
RobotControlPacket receivedPacket; 
  if(huart->Instance==USART3)
  {
    receivedPacket.frameHeader = receivedBuffer[0];
    receivedPacket.keyState = receivedBuffer[1];
    receivedPacket.joystickX = receivedBuffer[2];
    receivedPacket.joystickY = receivedBuffer[3];
    receivedPacket.frameTail = receivedBuffer[4];

		/*HAL_UART_Receive_DMA(&huart3,RxData,5);
    if(RxData[0]==0xA5&&RxData[4]==0x5A)
    {
      uint8_t keyState=RxData[1];
			int Base1=800,Base2=300;
			switch(keyState)
			{
				case 0x01:
					motor_direction_forward();
				  PWMA(Base1);
				  PWMB(Base1);
          motor_direction_forward();
          motorL_Target_speed(motor1_pulse,Base1);
          motorR_Target_speed(motor2_pulse,Base1);
          PWMB(speed_left_pid_pwm(motor1_pulse));
          PWMA(speed_right_pid_pwm(motor2_pulse));
				break;
				case 0x02:
					motor_direction_back();
				  PWMA(Base1);
				  PWMB(Base1);
          motorL_Target_speed(motor1_pulse,Base1);
          motorR_Target_speed(motor2_pulse,Base1);
          PWMB(speed_left_pid_pwm(motor1_pulse));
          PWMA(speed_right_pid_pwm(motor2_pulse));
				break;
				case 0x04:
					motor_direction_left();
				  PWMA(Base2);
				  PWMB(Base2);
          motorL_Target_speed(motor1_pulse,Base2);
          motorR_Target_speed(motor2_pulse,Base2);
          PWMB(speed_left_pid_pwm(motor1_pulse));
          PWMA(speed_right_pid_pwm(motor2_pulse));
				break;
				case 0x10:
					motor_direction_right();
				  PWMA(Base2);
				  PWMB(Base2);
          motorL_Target_speed(motor1_pulse,Base2);
          motorR_Target_speed(motor2_pulse,Base2);
          PWMB(speed_left_pid_pwm(motor1_pulse));
          PWMA(speed_right_pid_pwm(motor2_pulse));
				break;
				default:
					break;
			}
			HAL_UART_Receive_DMA(&huart3,RxData,5);//重新接受新一轮数�?
    }*/
  }
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
