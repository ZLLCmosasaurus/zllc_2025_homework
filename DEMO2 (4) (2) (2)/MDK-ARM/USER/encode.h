#include "gpio.h"

#include "tim.h"

#include "stm32f1xx.h"





#ifndef ENCODE_H

#define ENCODE_H
#ifdef __cplusplus
extern "C" {
#endif


	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
// 编码器结构体定义
typedef struct {
    GPIO_TypeDef* A_port;  // A信号对应GPIO端口
    uint16_t A_pin;        // A信号对应GPIO引脚
    GPIO_TypeDef* B_port;  // B信号对应GPIO端口
    uint16_t B_pin;        // B信号对应GPIO引脚
    int32_t count;         // 编码器计数值
    uint8_t last_state;    // 上一次状态
    uint32_t last_time;    // 上次中断时间
} Encoder;
extern Encoder encoders[4];
//轮子周长
  extern  float wheel_circumference ;

    // 编码器比例因子
   extern float encoder_scale ;
#ifdef __cplusplus
}
#endif
#endif


