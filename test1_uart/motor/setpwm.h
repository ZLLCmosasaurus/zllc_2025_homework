#ifndef SETPWM_H
#define SETPWM_H
#ifdef __cplusplus
extern "C" {
#endif

#include "pid.h"
#include "esp32.h"
#include "tim.h"
#include "motor.h"
#include "gpu.h"	
#define ENCODER_PPR 209    //减速比*每圈脉冲数

#define TIME_INTERVAL 0.05   //时间间隔s

#define pwm_max 100

extern float max_v[4];  //最大计数值为0X8C 十进制140
extern int pwm[4];
extern int pwm_set[4];
int setpwm(IncrementalPID pid,float SV,float PV,int i,usart_1* esp32);

void steering(usart_1 *s);
	

#ifdef __cplusplus
}
#endif

#endif 
