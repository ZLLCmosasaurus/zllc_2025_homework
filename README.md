# 碱基互补配对队软件设计
## 下位机部分
### 系统架构
   利用CubeMX，Keil5软件，基于HAL库开发
### 运行流程
通过摇杆传回的数值对速度目标函数void PID_Init_1(PID_TypeDef* pid, float Kp_1, float Ki_1, float Kd_1, float setpoint_1)实时进行改变，并利用PID算法调整电机转速，实现对电机的闭环控制。
### 重点功能
   受限于stm32f103c8t6资源限制，并未采用定时器中的编码器模式，而是利用IO口的EXTI模式，设置为上升沿触发中断，对计数值进行++操作，进而计算电机转速，并在void ControlMotorSpeed_3(float speed)函数中传入pid计算函数float PID_Compute_moto1(PID_TypeDef*pid)的数值来进行PWM占空比的调试，至于电机方向函数，考虑到实际操作方便性，只针对315°~45°，135°~225°进行了连续性设置，其余角度均改为了原地左转或右转（约占摇杆整体的20%），具体方向改变措施则是通过反转连接tb6612的IN口电平来实现的
### 软件测试
  连接小车调试后得到以下数据：
  满预分频值（99）下对应满转速大约为400
  摇杆数据（来自于esp32）调节占空比的系数约为8
  Kp≈1.0   Ki≈0.1   Kd≈0.05
## 通信部分
### 系统架构
使用串口2的Rx与Tx实现对ESP32数据的收发，并在HAL_UART_RxCpltCallback回调函数中实现对ESP32数据的解包以便进行下一步处理；
使用串口3的Rx与Tx实现对上位机数据的收发，并在HAL_UARTEx_RxEventCallback回调函数中实现对上位机数据的解包。
### 运行流程
首先在main（）中用HAL_UARTEx_ReceiveToIdle_DMA(&huart3, xUSART_PC.BuffTemp, 14)、HAL_UART_Receive_IT(&huart2,receive_data, 5)将回调函数使能，为防止在上位机通讯时HAL_UART_Receive_IT进入HAL_UARTEx_RxEventCallback回调函数导致程序卡死，加入__HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);再通过缓冲区接收数据、判断帧头帧尾后将数据存储到相应的结构体成员变量中。
### 重点功能
拨动摇杆实现对小车方向的控制；
按下JUMP键向上位机发送切换的指示，上位机再发送0x01实现对下位机的控制；
上位机发送相应的数据包在下位机解析后实现对小车的控制。
### 软件测试
连接小车后进入Debugger窗口观测收发数据；
能够正常收发与解析数据包