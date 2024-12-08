#include "motor.h"
void Motor_SetSpeed1(int8_t Speed)
{
    if (Speed >= 0)
    {
        // 设置方向为正转
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Speed);
    }
    else
    {
        // 设置方向为反转
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, -Speed);
    }
}

void Motor_SetSpeed2(int8_t Speed)
{
    if (Speed >= 0)
    {
        // 设置方向为正转
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, Speed);
    }
    else
    {
        // 设置方向为反转
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, -Speed);
    }
}

void Motor_SetSpeed3(int8_t Speed)
{
    if (Speed >= 0)
    {
        // 设置方向为正转
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, Speed);
    }
    else
    {
        // 设置方向为反转
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, -Speed);
    }
}

void Motor_SetSpeed4(int8_t Speed)
{
    if (Speed >= 0)
    {
        // 设置方向为正转
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, Speed);
    }
    else
    {
        // 设置方向为反转
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, -Speed);
    }
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_6)  // 检测 A 相信号的变化
    {
        // 检查 A 相当前状态（上升沿或下降沿）
        GPIO_PinState a_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
        // 读取 B 相电平
        GPIO_PinState b_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);


        // 四倍频计数逻辑
        if (a_state == GPIO_PIN_SET)  // A 相上升沿
        {
            if (b_state == GPIO_PIN_SET)
                encoder_count[0]++;  // 顺时针计数
            else
                encoder_count[0]--;  // 逆时针计数
        }
        else  // A 相下降沿
        {
            if (b_state == GPIO_PIN_RESET)
                encoder_count[0]++;  // 顺时针计数
            else
                encoder_count[0]--;  // 逆时针计数
        }
    }
    else if (GPIO_Pin == GPIO_PIN_7)  // 检测 A 相信号的变化
    {
        // 检查 A 相当前状态（上升沿或下降沿）
        GPIO_PinState a_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
        // 读取 B 相电平
        GPIO_PinState b_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);


        // 四倍频计数逻辑
        if (a_state == GPIO_PIN_SET)  // A 相上升沿
        {
            if (b_state == GPIO_PIN_SET)
                encoder_count[1]++;  // 顺时针计数
            else
                encoder_count[1]--;  // 逆时针计数
        }
        else  // A 相下降沿
        {
            if (b_state == GPIO_PIN_RESET)
                encoder_count[1]++;  // 顺时针计数
            else
                encoder_count[1]--;  // 逆时针计数
        }
    }
    else if (GPIO_Pin == GPIO_PIN_8)  // 检测 A 相信号的变化
    {
        // 检查 A 相当前状态（上升沿或下降沿）
        GPIO_PinState a_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
        // 读取 B 相电平
        GPIO_PinState b_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);


        // 四倍频计数逻辑
        if (a_state == GPIO_PIN_SET)  // A 相上升沿
        {
            if (b_state == GPIO_PIN_SET)
                encoder_count[2]++;  // 顺时针计数
            else
                encoder_count[2]--;  // 逆时针计数
        }
        else  // A 相下降沿
        {
            if (b_state == GPIO_PIN_RESET)
                encoder_count[2]++;  // 顺时针计数
            else
                encoder_count[2]--;  // 逆时针计数
        }
    }
    else if (GPIO_Pin == GPIO_PIN_9)  // 检测 A 相信号的变化
    {
        // 检查 A 相当前状态（上升沿或下降沿）
        GPIO_PinState a_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
        // 读取 B 相电平
        GPIO_PinState b_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);


        // 四倍频计数逻辑
        if (a_state == GPIO_PIN_SET)  // A 相上升沿
        {
            if (b_state == GPIO_PIN_SET)
                encoder_count[3]++;  // 顺时针计数
            else
                encoder_count[3]--;  // 逆时针计数
        }
        else  // A 相下降沿
        {
            if (b_state == GPIO_PIN_RESET)
                encoder_count[3]++;  // 顺时针计数
            else
                encoder_count[3]--;  // 逆时针计数
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)  // 定时器 2 的中断
    {
        for (int i = 0; i < 4; i++)
        {
            // 暂存当前计数值
            int32_t current_count = encoder_count[i];

            // 计算增量计数
            delta_count[i] = current_count;

            // 清空 encoder_count
            encoder_count[i] = 0;

            // 计算 RPS（转每秒）
            speed_rps[i] = (float)delta_count[i] / (4 * 13) / 0.01f;
            speed[i]=speed_rps[i]/3.5;
            if(speed[i]>100)
            {
                speed[i]=100;
            }
            else if(speed[i]<-100)
            {
                speed[i]=-100;
            }
        }
    }
}