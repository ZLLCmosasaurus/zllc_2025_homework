#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>
#include "stm32f103xb.h"
#include "tim.h"

struct Motor
{
    GPIO_TypeDef * GPIOX_M; // 记录电机选择正反转引脚编号
    uint16_t GPIO_PIN_Motor;

    uint32_t Count;
    uint8_t Number;
		int16_t sum;
    int16_t Speed[4]; // 记录两次速度，均值滤波，除去因定时器进入时机导致的编码器计数值存在正负偏差1

    int16_t Expect_Speed;
		
		float Add_PWM;
    float Kp;
    float Ki;
    float Kd;
    float PWM;
    float error[3];
};


extern struct Motor M1, M2, M3, M4;
extern struct Motor * M0; // M0用于能在中断里更新Speed
extern uint8_t Now_Motor;

#ifdef __cplusplus
extern "C" {
#endif
void Motor_Init();						
void Struct_Init();
void Get_Speed(struct Motor *M);
void Set_ExSpeed(struct Motor *M, int16_t Expect_Speed);
void Set_Speed(struct Motor *M,int16_t PWM);
void Turn_Motor(struct Motor *M,uint8_t flag);
void PID_Motor(struct Motor *M);
void PID_Set(float Kp, float Ki, float Kd);

#ifdef __cplusplus
}
#endif

#endif