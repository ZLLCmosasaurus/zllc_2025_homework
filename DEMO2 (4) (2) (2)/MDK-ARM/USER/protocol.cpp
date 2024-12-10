#include "protocol.h"
#include "motor.h"
#include "encode.h"
#include <usart.h>
 //DMA 缓冲区
static uint8_t dma_rx_buffer[BUFFER_SIZE];
static uint8_t dma_rx_buffer_up[BUFFER_SIZE];
//static uint8_t packet_buffer[PACKET_SIZE];
static uint16_t last_read_index = 0;

extern UART_HandleTypeDef huart2;

// DMA 初始化

void DMA_UART_Init(void) {
    HAL_UART_Receive_DMA(&huart2, dma_rx_buffer, BUFFER_SIZE); // 启用 DMA 接收
    HAL_UART_Receive_DMA(&huart3,dma_rx_buffer_up,BUFFER_SIZE);
}

// DMA 数据处理 下位机
void DMA_UART_Process(void)
{
    static uint8_t packet_buffer[PACKET_SIZE]; // 临时缓冲区，用于存储当前帧
    static uint16_t packet_index = 0;         // 当前帧缓冲区索引

    // 获取当前 DMA 接收位置
    uint16_t current_index = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart2.hdmarx);

    // 循环处理数据
    while (last_read_index != current_index) {
        // 从环形 DMA 缓冲区读取数据
        uint8_t byte = dma_rx_buffer[last_read_index];
        last_read_index = (last_read_index + 1) % BUFFER_SIZE;

        // 如果当前帧缓冲区为空，寻找帧头
        if (packet_index == 0) {
            if (byte == FRAME_HEADER) {
                packet_buffer[packet_index++] = byte; // 找到帧头，开始记录数据
            }
        } else {
            // 将数据存入临时缓冲区
            packet_buffer[packet_index++] = byte;

            // 检查缓冲区是否已满
            if (packet_index == PACKET_SIZE) {
                // 检查帧尾
                if (packet_buffer[PACKET_SIZE - 1] == FRAME_TAIL) {
                    // 数据包解析
                    ControlData controlData;
                    controlData.FORWARD = (packet_buffer[1] & 0x01);
                    controlData.BACK = (packet_buffer[1] & 0x02) >> 1;
                    controlData.JUMP = (packet_buffer[1] & 0x04) >> 2;
                    controlData.LEFT = (packet_buffer[1] & 0x08) >> 3;
                    controlData.RIFGT = (packet_buffer[1] & 0x10) >> 4;
                    controlData.ROBOT_GO = (packet_buffer[1] & 0x20) >> 5;
                    controlData.joystickX = packet_buffer[2];
                    controlData.joystickY = packet_buffer[3];

                    // 调用数据处理函数
                    Process_ControlData(&controlData);
                }
// 无论数据包是否有效，都清空缓冲区以准备下一帧
                packet_index = 0;
								
                
				}
            }
		}//以下为开环测试，上车时删除
ComputeWheelSpeeds(Vc, Wc, WHEEL_BASE);
        for (int i = 0; i < 4; i++) {
            

            float current_speed = encoders[i].count * encoder_scale;  // ENCODER_SCALEæ ¹æ®ç¼–ç å™¨åˆ†è¾¨çŽ‡å’Œå‡é€Ÿæ¯”è®¡ç®—
            encoders[i].count = 0;  // æ¸…é›¶è®¡æ•°
            float pwm =(target_speed[i]/Vmax)*99;
            pwm=SET_MOTOR(i, pwm);//
            SetMotorPWM(i, pwm);//
    }

}
/*
void DMA_UART_Process_up(void)
{
    static uint8_t packet_buffer[PACKET_SIZE]; // 临时缓冲区，用于存储当前帧
    static uint16_t packet_index = 0;         // 当前帧缓冲区索引

    // 获取当前 DMA 接收位置
    uint16_t current_index = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart3.hdmarx);

    // 循环处理数据
    while (last_read_index != current_index) {
        // 从环形 DMA 缓冲区读取数据
        uint8_t byte = dma_rx_buffer[last_read_index];
        last_read_index = (last_read_index + 1) % BUFFER_SIZE;

        // 如果当前帧缓冲区为空，寻找帧头
        if (packet_index == 0) {
            if (byte == FRAME_HEADER) {
                packet_buffer[packet_index++] = byte; // 找到帧头，开始记录数据
            }
        } else {
            // 将数据存入临时缓冲区
            packet_buffer[packet_index++] = byte;

            // 检查缓冲区是否已满
            if (packet_index == PACKET_SIZE) {
                // 检查帧尾
                if (packet_buffer[PACKET_SIZE - 1] == FRAME_TAIL) {
                    // 数据包解析


                    // 调用数据处理函数
                    
                }

                // 无论数据包是否有效，都清空缓冲区以准备下一帧
                packet_index = 0;
            }
        }
    }
}
*/
/*
void DMA_UART_Process(void) {
    uint16_t current_index = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart2.hdmarx); // 获取当前接收位置


	while (last_read_index != current_index) {
        // 复制数据到临时缓冲区
        packet_buffer[last_read_index % PACKET_SIZE] = dma_rx_buffer[last_read_index];
        last_read_index = (last_read_index + 1) % BUFFER_SIZE;

        // 检查是否有完整帧
        if (packet_buffer[0] == FRAME_HEADER && packet_buffer[4] == FRAME_TAIL) {
            ControlData controlData;
            //controlData.buttonState = packet_buffer[1];
            controlData.FORWARD=(packet_buffer[1]&0x01);
	        controlData.BACK=(packet_buffer[1]&0x02)>>1;
	        controlData.JUMP=(packet_buffer[1]&0x04)>>2;
	        controlData.LEFT=(packet_buffer[1]&0x08)>>3;
	        controlData.RIFGT=(packet_buffer[1]&0x10)>>4;
	        controlData.ROBOT_GO=(packet_buffer[1]&0x20)>>5;
            controlData.joystickX = packet_buffer[2];
            controlData.joystickY = packet_buffer[3];
            Process_ControlData(&controlData); // 处理数据
        }
    }
}
*/
/*
void DMA_UART_Process(void)
{
    static uint8_t parse_buffer[PACKET_SIZE];
    static uint16_t last_pos = 0;
    uint16_t current_pos = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx); // 获取 DMA 当前写入位置

    // 逐字节检查
    while (last_pos != current_pos) {
        uint8_t byte = DMA_RxBuffer[last_pos];

        // 检测帧头
        if (byte == FRAME_HEADER && buffer_index == 0) {
            parse_buffer[buffer_index++] = byte; // 开始帧收集
        } else if (buffer_index > 0) {
            parse_buffer[buffer_index++] = byte;

            // 收集到完整帧
            if (buffer_index == PACKET_SIZE) {
                if (parse_buffer[PACKET_SIZE - 1] == FRAME_TAIL) {
                    // 数据帧解析成功
                    control_data.FORWARD = parse_buffer[1] & 0x01;
                    control_data.BACK = parse_buffer[1] & 0x02;
                    control_data.LEFT = parse_buffer[1] & 0x04;
                    control_data.RIFGT = parse_buffer[1] & 0x08;
                    control_data.ROBOT_GO = parse_buffer[1] & 0x10;
                    control_data.joystickX = parse_buffer[2];
                    control_data.joystickY = parse_buffer[3];
                    control_data.buttonState = parse_buffer[4];
                }
                // 重置缓冲区
                buffer_index = 0;
            } else if (buffer_index >= PACKET_SIZE) {
                // 无效帧，丢弃
                buffer_index = 0;
            }
        }

        // 环形缓冲区索引递增
        last_pos = (last_pos + 1) % BUFFER_SIZE;
    }
}
*/
// 根据解析的数据计算目标线速度角速度
void Process_ControlData_up(ControlData_up *data){
    int x = data->V ;
    int y = data->W ;
    Vc+=x;
    Wc+=y;
    }
void Process_ControlData(ControlData *data){
    int x = data->joystickX - JOYSTICK_MID-(data->LEFT*100)+(data->RIFGT*100); 
    int y = data->joystickY - JOYSTICK_MID+(data->FORWARD*100)-(data->BACK*100); 
    Vc=Vmax*y;
    Wc=Wmax*x;
    }
// void Parse_esp32(usart_1 *uart1,uint8_t esp32_data[5]){
// 	uart1->FORWARD=(esp32_data[1]&0x01);
// 	uart1->BACK=(esp32_data[1]&0x02)>>1;
// 	uart1->JUMP=(esp32_data[1]&0x04)>>2;
// 	uart1->LEFT=(esp32_data[1]&0x08)>>3;
// 	uart1->RIFGT=(esp32_data[1]&0x16)>>4;
// 	uart1->ROBOT_GO=(esp32_data[1]&0x32)>>5;
// 	uart1->location_x=esp32_data[2];
// 	uart1->location_y=esp32_data[3];
// 	uart1->z=uart1->location_x*uart1->location_x+uart1->location_y*uart1->location_y;
// 	}//按钮赋值

    
