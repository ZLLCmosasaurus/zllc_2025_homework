#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"
extern TIM_HandleTypeDef htim3;
extern int32_t encoder_count[4];

extern int32_t delta_count[4] ;            // 每次定时器周期的增量计数值
extern float speed_rps[4] ;             // 转速（RPS）
extern float speed[4] ;
void Motor_SetSpeed1(int8_t Speed);
void Motor_SetSpeed2(int8_t Speed);
void Motor_SetSpeed3(int8_t Speed);
void Motor_SetSpeed4(int8_t Speed);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


#endif // MOTOR_H
