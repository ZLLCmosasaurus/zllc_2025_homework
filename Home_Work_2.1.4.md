# UART

- Universal Asynchronous Receiver Transmitter(通用异步收发器)
-  ==一种用于串行数据传输的硬件电路,允许不同设备之间进行异步通信==
- 三线通信
	- TX 与 RX 交叉接,必须共地
- 需设置相同波特率、数据位、停止位和奇偶的校验
- 信号须经过电平转换芯片转换为`TTL电平`才可进行通讯
- 数据结构
	- 起始位
		- 持续一个比特时间的逻辑0电平
	- 数据位
		- 传输数据时先传送字符的低位，后传送字符的高位
	- 奇偶校验位
		- 占一位(奇校验需要保证传输的数据总共有奇数个逻辑高位)
	- 停止位
		- 逻辑1电平，标志着传输一个字符的结束
	- 空闲位
		- 线路处于空闲状态，必须由高电平来填充

# DMA

- Direct Memory Access(直接存储器访问)
- ==一种硬件机制，允许外部设备直接与系统内存进行数据交换，无需CPU介入==
- 必须明确数据的源地址，数据传输位置的目标地址，传递数据多少的数据传输量，进行多少次传输的传输模式
- DMA 控制器
	- 有8个数据流,同时处理8个不同的数据传输任务
	- 每个通道都有3个事件标志（DMA半传输、DMA传输完成和DMA传输出错）
	- 源数据先送入FIFO，达到FIFO的触发阈值后，再传送到目标地址
- DMA数据传输方式
	- 普通模式
		- 传输结束后（即要传输数据的数量达到零），将不再产生DMA操作。若要开始新的DMA传输，需在关闭DMA通道情况下，重新启动DMA传输
	- 循环模式
		- 循环模式：可用于处理环形缓冲区和连续数据流。每轮传输结束时，要传输的数据数量将自动用设置的初始值进行加载，并继续响应DMA请求

```cpp
//初始化
void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length){
  //在接收到空闲信号后停止接收，并触发中断
  HAL_UARTEx_ReceiveToIdle_DMA();
  //避免半传输中断
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}
//DMA 空闲中断
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)


```



# Dbus

- 遥控器和 stm32 之间采用 DBUS 协议进行通讯
- 传输速率为 100k bit/s，数据长度为 8 位，奇偶校验位为偶校验，结束位 1 位
- DBUS与UART之间电平标准为反相关系(可直接接到 C 板的串口 1处,因为其接口内置了反相器)

