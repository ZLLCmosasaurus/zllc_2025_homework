- # CAN协议

为什么要使用CAN通信  

其他通信协议如UART一般只能用在两个设备之间，而CAN通信引入了ID的概念从而能够实现在同一主线上与多个设备之间进行通信（且比赛中大疆电机都是用CAN控制的）

CAN的通信格式

CAN是靠帧来进行通信传递，帧分为数据帧和遥控帧，以数据帧为例：一个完整的数据帧为<u>帧起始+仲裁场+控制场+数据场+CRC场+应答场+帧结尾+帧间隔</u>。对我们来说主要只用看仲裁场和数据场，在仲裁场内设定标识符（ID）判断是否接受该数据，而数据场内的数据才是实际需要传递的信息。

CAN的使用（实验室框架）

Ⅰ.定义缓冲区（示例）

```c_cpp
uint8_t CAN1_0x1ff_Tx_Data[8];
uint8_t CAN1_0x200_Tx_Data[8];
uint8_t CAN1_0x2ff_Tx_Data[8];
```

Ⅱ.配置过滤器（CAN初始化之前必须配置过滤器，无论是否需要过滤）

```c_cpp
/**
 * @brief 配置CAN的过滤器
 *
 * @param hcan CAN编号
 * @param Object_Para 筛选器编号0-27 | FIFOx | ID类型 | 帧类型
 * @param ID 验证码
 * @param Mask_ID 屏蔽码(0x3ff, 0x1fffffff)
 */
void can_filter_mask_config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID)
```

Ⅲ.初始化函数

```c_cpp
/**
 * @brief CAN接收的信息结构体
 *
 */
struct Struct_CAN_Rx_Buffer
{
    CAN_RxHeaderTypeDef Header;
    uint8_t Data[8];
};

/**
 * @brief CAN通信接收回调函数数据类型
 *
 */
typedef void (*CAN_Call_Back)(Struct_CAN_Rx_Buffer *);
/**
 * @brief 初始化CAN总线
 *
 * @param hcan CAN编号
 * @param Callback_Function 处理回调函数
 */
void CAN_Init(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function)
```

  在CAN_Init函数里还有

```c_cpp
/*离开初始模式*/
  HAL_CAN_Start(hcan);				
/*开中断*/
  HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);       //can 接收fifo 0不为空中断
	HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);       //can 接收fifo 1不为空中断
```

  Ⅳ.定时发送函数

```c_cpp
/**
 * @brief CAN的TIM定时器中断发送回调函数
 *
 */
void TIM_CAN_PeriodElapsedCallback()
```

  在这个函数中嵌套使用了发送数据帧函数

```c_cpp
/**
 * @brief 发送数据帧
 *
 * @param hcan CAN编号
 * @param ID ID
 * @param Data 被发送的数据指针
 * @param Length 长度
 * @return uint8_t 执行状态
 */
uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length)
```

  返回值是直接给赋值到了HAL库发送数据的函数

```c_cpp
return (HAL_CAN_AddTxMessage(hcan, &tx_header, Data, &used_mailbox));
```

Ⅴ.接收中断函数

```c_cpp
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
```

***

- # djimotor的驱动（以C620为例）

Ⅰ.分配缓冲区：把缓冲区与电机的ID对应起来

```c_cpp
uint8_t *allocate_tx_data(CAN_HandleTypeDef *hcan, Enum_DJI_Motor_ID __CAN_ID)
```

Ⅱ.电机及其CAN初始化

```c_cpp
void Class_DJI_Motor_C620::Init(CAN_HandleTypeDef *hcan, Enum_DJI_Motor_ID __CAN_ID, Enum_DJI_Motor_Control_Method __DJI_Motor_Control_Method, float __Gearbox_Rate, float __Torque_Max)
```

Ⅲ.TIM定时器中断定期检测电机是否存活

```c_cpp
void Class_DJI_Motor_C620::TIM_Alive_PeriodElapsedCallback()
```

Ⅳ.TIM定时器中断计算回调函数

```c_cpp
void Class_DJI_Motor_C620::TIM_PID_PeriodElapsedCallback()
```

  在这个函数里通过对电机控制模式的分别进行PID控制，同时嵌套使用了多个函数

***

- # PID

主要两个函数

```c_cpp
void Class_PID::Init(float __K_P, float __K_I, float __K_D, float __K_F, float __I_Out_Max, float __Out_Max, float __I_Variable_Speed_A, float __I_Variable_Speed_B, float __I_Separate_Threshold,float __D_T, float __Dead_Zone,  Enum_PID_D_First __D_First)
void Class_PID::TIM_Adjust_PeriodElapsedCallback()
```

具体函数内部（略），直接调用这两个函数就可以了