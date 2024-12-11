#ifndef MOTOR_H
#define MOTOR_H

void Motor_SetSpeed(TIM_HandleTypeDef *htim, uint32_t channel, int16_t target_value, GPIO_TypeDef *dir_port, uint16_t dir_pin, GPIO_TypeDef *dir_port1, uint16_t dir_pin1);


#endif
