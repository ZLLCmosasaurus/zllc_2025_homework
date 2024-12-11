#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "usart.h"
#include "tim.h"

void encoder_Init(void);
int Get_motorL_Encoder(int motor1_pulse);
int Get_motorR_Encoder(int motor2_pulse);
float speed_left_pid_control(int motor1_pulse);
float speed_right_pid_control(int motor2_pulse);
float speed_left_pid_pwm(int motor1_pulse);
float speed_right_pid_pwm(int motor2_pulse);
float motorL_Target_speed(int motor1_pulse,int Target_pwm);
float motorR_Target_speed(int motor2_pulse,int Target_pwm);
void motorControlL(uint8_t motor_state);
void motorControlR(uint8_t motor_state);



#define PWMA(speedR) __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, speedR)
#define PWMB(speedL) __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, speedL)

#endif
