#include "main.h"

#include "stm32f1xx.h"

#include "encode.h"

//convey 4 CNT

// 计数值取值

int GetCurrentMicros(void) {
	
    return __HAL_TIM_GET_COUNTER(&htim1);  // ���� TIM1 ��Ϊʱ���׼��Ƶ��Ϊ 1 MHz
	
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    GPIO_PinState B_state;

    // ʹ�� switch ���ж��ĸ��������� A ���źŴ���
    switch (GPIO_Pin) {
        case GPIO_PIN_6: // ������ 0 �� A ���ź�
            B_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7); // ��ȡ B ��״̬'
            if (B_state == GPIO_PIN_SET) {
                encoder_count[0]++; // ˳ʱ��
            } else {
                encoder_count[0]--; // ��ʱ��
            }
            break;

        case GPIO_PIN_5: // ������ 1 �� A ���ź�
            B_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4); // ��ȡ B ��״̬
            if (B_state == GPIO_PIN_SET) {
                encoder_count[1]++; // ˳ʱ��
            } else {
                encoder_count[1]--; // ��ʱ��
            }
            break;

        case GPIO_PIN_12: // ������ 2 �� A ���ź�
            B_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13); // ��ȡ B ��״̬
            if (B_state == GPIO_PIN_SET) {
                encoder_count[2]++; // ˳ʱ��
            } else {
                encoder_count[2]--; // ��ʱ��
            }
            break;

        case GPIO_PIN_0: // ������ 3 �� A ���ź�
            B_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1); // ��ȡ B ��״̬
            if (B_state == GPIO_PIN_SET) {
                encoder_count[3]++; // ˳ʱ��
            } else {
                encoder_count[3]--; // ��ʱ��
            }
            break;

        default:
            // δ֪�ж��źţ�����
            break;
    }
}
