#include<stdio.h>
#include<stdint.h>
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "motor.h"
#include "PID.h"
#include "data_transmit.h"

void Motor_SetSpeed(TIM_HandleTypeDef *htim, uint32_t channel, int speed, GPIO_TypeDef *dir_port, uint16_t dir_pin, GPIO_TypeDef *dir_port1, uint16_t dir_pin1) {
    if (speed > 0) {
        HAL_GPIO_WritePin(dir_port, dir_pin, GPIO_PIN_SET); // 正转
         HAL_GPIO_WritePin(dir_port1, dir_pin1, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(htim, channel, speed);
    } else {
        HAL_GPIO_WritePin(dir_port, dir_pin, GPIO_PIN_RESET); // 反转
         HAL_GPIO_WritePin(dir_port1, dir_pin1, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(htim, channel, -speed);
    }
}

