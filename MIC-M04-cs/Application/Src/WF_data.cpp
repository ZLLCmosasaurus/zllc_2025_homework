#include "WF_data.h"
#include "math.h"
void WF_ControlCar(uint8_t data[5], int speed[4]) {
    // 检查按键状态
    if (data[1] & (1 << 0)) {
        Move_Front(speed); // 前进
        return;
    }
    if (data[1] & (1 << 1)) {
        Move_Back(speed); // 后退
        return;
    }
    if (data[1] & (1 << 4)) {
        Move_Left(speed); // 左转
        return;
    }
    if (data[1] & (1 << 3)) {
        Move_Right(speed); // 右转
        return;
    }
    if (data[1] & (1 << 2)) {
        Move_Stop(); // 停止
        return;
    }
    if (data[1] & (1 << 5)) {
        // 其他模式控制逻辑
        return;
    }

    // 处理摇杆输入
    if (data[2] != 100 || data[3] != 100) {
        // 映射摇杆输入 [-100, 100]，中心点为 (100, 100)
        int x = 100 - data[2]; // >0 左转，<0 右转
        int y = data[3] - 100; // >0 前进，<0 后退

        // 调整灵敏度（减少轻微偏移导致的误动作）
        float sensitivity = 1.2f; // 灵敏度调整，值越高越敏感
        x = (int)(x * sensitivity);
        y = (int)(y * sensitivity);

        // 根据摇杆值选择方向
        if (y > 0 && abs(y) > abs(x)) {
            // 前进，所有电机以相同速度 y
            int tempspeed[4] = {y, y, y, y};
            Move_Front(tempspeed);
        } else if (y < 0 && abs(y) > abs(x)) {
            // 后退，所有电机以相同速度 -y
            int tempspeed[4] = {-y, -y, -y, -y};
            Move_Back(tempspeed);
        } else if (x > 0 && abs(x) > abs(y)) {
            // 右转，所有电机以相同速度 x
            int tempspeed[4] = {x, x, x, x};
            Move_LTwist(tempspeed);
        } else if (x < 0 && abs(x) > abs(y)) {
            // 左转，所有电机以相同速度 -x
            int tempspeed[4] = {-x, -x, -x, -x};
            Move_RTwist(tempspeed);
        } else {
            // 停止所有电机
            Move_Stop();
        }
    } else {
        // 摇杆未操作时小车静止
        Move_Stop();
    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        // 将接收到的数据存入环形缓冲区
        rx_buffer[write_index] = rx_byte;
        write_index = (write_index + 1) % BUFFER_SIZE;

        // 继续接收下一个字节
        HAL_UART_Receive_IT(&huart3, &rx_byte, 1);
    }
    if (huart->Instance == USART2) {  // 确认是USART1
        //执行中断操作开始
        HAL_UART_Transmit_IT(&huart2, receiveData, 2);
        //执行中断操作结束
        HAL_UART_Receive_IT(&huart2, receiveData, 2);
    }
}


void ProcessReceivedData(void) {
    // 当缓冲区中存在未处理的数据时循环
    while ((write_index != read_index) && !frame_received) {
        // 如果找到帧头
        if (rx_buffer[read_index] == FRAME_HEAD) {
            // 确保缓冲区内有足够的数据来检查帧的完整性
            if (((write_index + BUFFER_SIZE - read_index) % BUFFER_SIZE) >= DATA_LEN) {
                uint16_t temp_index = read_index;
                uint8_t valid_frame = 1;

                // 检查帧中间数据完整性
                for (int i = 0; i < DATA_LEN - 1; i++) {
                    data_buffer[i] = rx_buffer[temp_index];
                    temp_index = (temp_index + 1) % BUFFER_SIZE;
                }

                // 检查帧尾是否正确
                if (rx_buffer[temp_index] == FRAME_TAIL) {
                    data_buffer[DATA_LEN - 1] = rx_buffer[temp_index];
                    frame_received = 1; // 接收完整帧标志
                    read_index = (read_index + DATA_LEN) % BUFFER_SIZE; // 更新读取索引
                } else {
                    valid_frame = 0;
                }

                // 如果帧无效，跳过当前帧头
                if (!valid_frame) {
                    read_index = (read_index + 1) % BUFFER_SIZE;
                }
            } else {
                // 缓冲区数据不足以组成一帧，退出循环
                break;
            }
        } else {
            // 非帧头字节，直接跳过
            read_index = (read_index + 1) % BUFFER_SIZE;
        }
    }
}

