
#include "gpio.h"

#include "tim.h"

#include "stm32f1xx.h"

#include "encode.h"
#include "motor.h"
//编码器外部中断处�???
//用于捕获每个轮子的增量式编码器信号（A/B相）
Encoder encoders[4] = {
    {GPIOA, GPIO_PIN_8, GPIOB, GPIO_PIN_12, 0, 0, 0}, // 左前轮
    {GPIOA, GPIO_PIN_9, GPIOB, GPIO_PIN_13, 0, 0, 0}, // 左后轮
    {GPIOA, GPIO_PIN_10, GPIOB, GPIO_PIN_14, 0, 0, 0}, // 右前轮
    {GPIOA, GPIO_PIN_11, GPIOB, GPIO_PIN_15, 0, 0, 0}  // 右后轮
};
// 消抖时间阈值（微秒）
#define DEBOUNCE_TIME_US 200

// 获取当前时间（微秒）
static uint32_t GetMicros(void) {
    return __HAL_TIM_GET_COUNTER(&htim3); // 假定TIM1已配置为1MHz
}

// 外部中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    uint32_t current_time = GetMicros();

     for (int i = 0; i < 4; i++) {
        if (encoders[i].B_pin == GPIO_Pin) { // 找到对应的编码器
            // 消抖判断
            if (current_time - encoders[i].last_time > DEBOUNCE_TIME_US) {
                encoders[i].last_time = current_time;

                // 读取当前A/B状态
                uint8_t current_state = 0;
                if (HAL_GPIO_ReadPin(encoders[i].A_port, encoders[i].A_pin)) {
                    current_state |= 0x01; // A高电平
                }
                if (HAL_GPIO_ReadPin(encoders[i].B_port, encoders[i].B_pin)) {
    
									current_state |= 0x02; // B高电平
                }

                // 根据状态变化更新计数
                if ((encoders[i].last_state == 0x00 && current_state == 0x01) ||
                    (encoders[i].last_state == 0x01 && current_state == 0x03) ||
                    (encoders[i].last_state == 0x03 && current_state == 0x02) ||
                    (encoders[i].last_state == 0x02 && current_state == 0x00)) {
                    encoders[i].count++; // 顺时针
                } else if ((encoders[i].last_state == 0x00 && current_state == 0x02) ||
                           (encoders[i].last_state == 0x02 && current_state == 0x03) ||
                           (encoders[i].last_state == 0x03 && current_state == 0x01) ||
                           (encoders[i].last_state == 0x01 && current_state == 0x00)) {
                    encoders[i].count--; // 逆时针
                }

                // 更新状态
                encoders[i].last_state = current_state;
            }
            break;
        }
    }
}
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint8_t current_state = 0;
    int wheel_index = -1;

    // 判断中断引脚属于哪个编码�???
    if (GPIO_Pin == GPIO_PIN_12) {
        wheel_index = 0;  // 编码�???1(左前)
    } else if (GPIO_Pin == GPIO_PIN_13) {
        wheel_index = 1;  // 编码�???2（左�???
    } else if (GPIO_Pin == GPIO_PIN_14) {
        wheel_index = 2;  // 编码�???3（右�???
    } else if (GPIO_Pin == GPIO_PIN_15) {
        wheel_index = 3;  // 编码�???4（右�???
    }

    if (wheel_index >= 0) {
        // 读取当前A/B相状�???
        switch (wheel_index)
        {
          case 0:
          if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)) {
            current_state |= 0x01; // A相高电平
          }
          if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)) {
             current_state |= 0x02; // B相高电平
          }
          break;
          case 1:
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) {
            current_state |= 0x01; // A相高电平
            }
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)) {
             current_state |= 0x02; // B相高电平
           }
          break;
        case 2:
                    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) {
            current_state |= 0x01; // A相高电平
          }
          if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)) {
             current_state |= 0x02; // B相高电平
          }
        break;
        case 3:
                  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)) {
            current_state |= 0x01; // A相高电平
          }
          if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)) {
             current_state |= 0x02; // B相高电平
          }
        break;

        }
        // 根据状�?�变化计算增�???
        if ((last_state[wheel_index] == 0x00 && current_state == 0x01) || 
            (last_state[wheel_index] == 0x01 && current_state == 0x03) || 
            (last_state[wheel_index] == 0x03 && current_state == 0x02) || 
            (last_state[wheel_index] == 0x02 && current_state == 0x00)) {
            encoder_count[wheel_index]++;
        } else if ((last_state[wheel_index] == 0x00 && current_state == 0x02) || 
                   (last_state[wheel_index] == 0x02 && current_state == 0x03) || 
                   (last_state[wheel_index] == 0x03 && current_state == 0x01) || 
                   (last_state[wheel_index] == 0x01 && current_state == 0x00)) {
            encoder_count[wheel_index]--;
        }

        // 更新状�????
        last_state[wheel_index] = current_state;
    }
}
*/
// 计算轮子周长


    // 计算编码器比例因子
float encoder_scale = wheel_circumference / (ENCODER_CPR * GEAR_RATIO);
