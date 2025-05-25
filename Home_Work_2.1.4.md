# Home_Work_2.1.4
## 一、drv_uart.cpp 的使用方法
### 1.头文件
`void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length);`初始化函数传一个句柄一个函数、接收缓冲区长度。
`uint8_t UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length);`
`void TIM_UART_PeriodElapsedCallback();`
### 2.源文件
- `UART_Init`将传入的huart指针、回调函数和接收缓冲区长度分别赋值给`UARTx_Manage_Object`结构体的相应成员。
使用`HAL_UARTEx_ReceiveToIdle_DMA`函数启动 DMA 接收，直到检测到 UARTx线路空闲。
使用`__HAL_DMA_DISABLE_IT`宏禁用 DMA 的半传输中断`（DMA_IT_HT）`。
- `UART_Send_Data`调用`HAL_UART_Transmit_DMA`函数，该函数是 STM32 HAL 库中用于通过 DMA 方式发送数据的函数。它会启动 DMA 传输，将Data指向的数据缓冲区中的Length个字节数据发送到huart指定的 UART 外设。
- `TIM_UART_PeriodElapsedCallback()`里套`UART_Send_Data`。
- `重写HAL_UARTEx_RxEventCallback`首先将本次接收的数据长度`Size`记录到`UARTx_Manage_Object.Rx_Length`中。
然后调用`UARTx_Manage_Object.Callback_Function`，将接收到的数据缓冲区`UARTx_Manage_Object.Rx_Buffer`和数据长度`Size作为参数传递，调用初始化时传入的函数。
最后，重新启动 DMA 接收，直到检测到 UART1 线路空闲，并禁用 DMA 的半传输中断DMA_IT_HT。
### 3.具体调用
- `Task_Init()`中调用了` UART_Init(&huart6, Referee_UART6_Callback, 128);`传入了`Referee_UART6_Callback`并将其存放在UARTx_Manage_Object中，实现了对`Referee_UART6_Callback`的调用。
- `Referee_UART6_Callback`内包`chariot.Referee.UART_RxCpltCallback(Buffer,Length);`实质上是将128传给了`Length`，`UARTx_Manage_Object.Rx_Buffer`传给了
``Buffer`,进入裁判系统的`Data_Process()`
## 二、串口的协议和DMA空闲中断使用方法
[串口通信视频](https://www.bilibili.com/video/BV1bc411J7Tv?vd_source=495d3ee5b051d595195ffb24e9b1d155)
[DMA空闲中断使用方法](https://www.bilibili.com/video/BV1do4y1F7wt?vd_source=495d3ee5b051d595195ffb24e9b1d155)
## 三、dvc_dr16.cpp 的使用方法
### 1.头文件
`Class_DR16`内包
`void Init(UART_HandleTypeDef *huart_1,UART_HandleTypeDef *huart_2);
    void DR16_UART_RxCpltCallback(uint8_t *Rx_Data);
    void Image_UART_RxCpltCallback(uint8_t *Rx_Data);
    void TIM1msMod50_Alive_PeriodElapsedCallback();
`
### 2.源文件
- `Init`通过检查 `huart_1 `所指向的 `UART `实例（`Instance `成员），如果是 `USARTx`，则将 `UART_Manage_Object_x` 指向 `UARTx_Manage_Object`。`UART_Manage_Object_x `是类 `Class_DR16` 中的一个成员变量，用于管理与` DR16 `相关串口的各种操作。
- `DR16_UART_RxCpltCallback`调用` DR16_Data_Process` 函数处理接收到的数据，最后将当前接收到的数据复制到` Pre_UART_Rx_Data` 中，以便后续与新接收的数据进行比较。
  `DR16_Data_Process`函数负责处理接收到的DR16遥控器数据。首先将接收到的数据从 `UART_Manage_Object_1->Rx_Buffer `复制到 `Now_UART_Rx_Dat`a 中。然后，它从 `UART_Manage_Object_1->Rx_Buffer`中提取摇杆数据，并根据`Rocker_Offset`和`Rocker_Num`进行转换，存储到`Data`结构体的相应成员中。接着，通过`Judge_Switch`函数判断拨码开关的状态变化。最后，处理左前轮相关信息（Yaw）并调用`Judge_Updata`函数判断数据是否更新。
`Judge_Switch`函数根据当前开关状态（Status）和上一次开关状态（Pre_Status）判断开关状态的变化，并将结果存储在 `*Switch `中。通过两层 switch 语句，对不同的前状态和当前状态组合进行判断，更新开关状态枚举值。
`Judge_Updata`函数通过比较 `Pre_UART_Rx_Data` 和 `Now_UART_Rx_Data `中摇杆通道数据（`Channel_0` 到 `Channel_3`），判断数据是否更新。如果所有通道数据都相同，则设置 `DR16_Updata_Status` 为 `DR16_Status_DisUpdata`，表示未更新；否则设置为 `DR16_Status_Updata`，表示已更新。
- `Image_UART_RxCpltCallback`函数在图像串口接收数据完成时被调用。首先判断帧头是否为 0xA5，如果是，则从接收到的数据中解析出命令 `ID（cmd_id`）和数据长度（`data_length`）。如果 `cmd_id` 为 0x0304 且 `data_length` 为 12，则递增 `Image_Flag`（用于判断遥控器是否在线），调用 `Image_Data_Process` 函数处理从` Rx_Data[7]` 开始的数据，并将这部分数据复制到 `Pre_UART_Image_Rx_Data` 中，以便后续与新接收的数据进行比较。
`Image_Data_Process`函数负责处理从图像串口接收到的数据。首先将接收到的数据从 `__rx_buffer` 复制到` Now_UART_Image_Rx_Data` 中。然后，从 `__rx_buffer` 中提取鼠标数据，并将其归一化到 [-1, 1] 范围存储到 `Data` 结构体的相应成员中。接着，通过 `Judge_Key` 函数判断鼠标左右键以及键盘按键的状态变化。
`Judge_Key`函数根据当前按键状态（`Status`）和上一次按键状态（`Pre_Status`）判断按键状态的变化，并将结果存储在 `*Key` 中。通过两层 switch 语句，对不同的前状态和当前状态组合进行判断，更新按键状态枚举值。
- `TIM1msMod50_Alive_PeriodElapsedCallback()`判断遥控器是否存活。
### 3.具体调用
- `Task_Init()`中调用了`UART_Init(&huart3, DR16_UART3_Callback, 18);`传入了`DR16_UART3_Callback`并将其存放在UARTx_Manage_Object中，实现了对`DR16_UART3_Callback`的调用。
- `DR16_UART3_Callback`内包`chariot.DR16.DR16_UART_RxCpltCallback(Buffer);`实质上是将18传给了`Length`，`UARTx_Manage_Object.Rx_Buffer`传给了
`Buffer`,进入`DR16_Data_Process();`
- `DR16_UART3_Callback`还内包`chariot.TIM_Control_Callback();`用以控制回调函数，具体还需进一步学习。
- - `Task_Init()`也调用了`UART_Init(&huart6, Image_UART6_Callback, 40);`传入了`Image_UART6_Callback`并将其存放在UARTx_Manage_Object中，实现了对`Image_UART6_Callback`的调用。
- `Image_UART6_Callback`内包` chariot.DR16.Image_UART_RxCpltCallback(Buffer);`实质上是将40传给了`Length`，`UARTx_Manage_Object.Rx_Buffer`传给了
`Buffer`
## 四、dbus接收机的通信协议和控制方法
[详解博客](https://blog.csdn.net/weixin_40026739/article/details/144029363?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_utm_term~default-1-144029363-blog-135593895.235^v43^pc_blog_bottom_relevance_base8&spm=1001.2101.3001.4242.1&utm_relevant_index=4)