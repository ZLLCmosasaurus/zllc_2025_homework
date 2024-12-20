
学习记录
学习记录1
can总线协议
1、帧的种类
数据帧：节点发送的包含ID和数据的帧，用于发送单元向接收单元传送数据的帧。

遥控帧：节点向网络上的其他节点发出的某个ID的数据请求，发送节点收到遥控帧后就可以发送相应ID的数据帧。

错误帧：节点检测出错误时，向其他节点发送的通知错误的帧。

过载帧：接收单元未做好接收数据的准备时发送的帧，发送节点收到过载帧后可以暂缓发送数据帧。

帧间空间：用于将数据帧、遥控帧与前后的帧分隔开的帧。
2、数据帧

帧起始：表示数据帧开始的段。
仲裁段：表示该帧优先级的段。
控制段：表示数据的字节数及保留位的段。
数据段：数据的内容，可发送 0～8 个字节的数据。
CRC 段：检查帧的传输错误的段。
ACK 段：表示确认正常接收的段。
帧结束：表示数据帧结束的段。

3、遥控帧
帧起始：表示帧开始的段。
仲裁段：表示该帧优先级的段。
控制段：表示数据的字节数及保留位的段。
CRC 段：检查帧的传输错误的段。
ACK 段：表示确认正常接收的段。
帧结束：表示遥控帧结束的段。

drv_can.cpp
/**
 * @brief 配置CAN的过滤器
 */
 void can_filter_mask_config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID);
 /**
 * @brief 初始化CAN总线
 */
 void CAN_Init(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function);
 /**
 * @brief 发送数据帧
 */
 uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length);
 /**
 * @brief CAN的TIM定时器中断发送回调函数
 */
 
 void TIM_CAN_PeriodElapsedCallback();
 /**
 * @brief HAL库CAN接收FIFO0中断
 */
 void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
 /**
 * @brief HAL库CAN接收FIFO1中断
 */
 void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
 
学习记录2
dvc_motor.cpp
主要内容：

/**
 * @brief 分配CAN发送缓冲区
 */
 uint8_t *allocate_tx_data(CAN_HandleTypeDef *hcan, Enum_DJI_Motor_ID __CAN_ID)
 /**
 * @brief 电机初始化
 *
 * @param hcan 绑定的CAN总线
 * @param __CAN_ID 绑定的CAN ID
 * @param __DJI_Motor_Control_Method 电机控制方式, 默认角度
 * @param __Encoder_Offset 编码器偏移, 默认0
 * @param __Omega_Max 最大速度, 需根据不同负载测量后赋值, 也就开环输出用得到, 不过我感觉应该没有奇葩喜欢开环输出这玩意
 */
 void Class_DJI_Motor_GM6020::Init(CAN_HandleTypeDef *hcan, Enum_DJI_Motor_ID __CAN_ID, Enum_DJI_Motor_Control_Method __DJI_Motor_Control_Method, int32_t __Encoder_Offset, float __Omega_Max)
 
/**
 * @brief 数据处理过程
 *
 */
 void Class_DJI_Motor_GM6020::Data_Process()
 /**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
 void Class_DJI_Motor_GM6020::Output()
 /**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
 void Class_DJI_Motor_GM6020::CAN_RxCpltCallback(uint8_t *Rx_Data)
 /**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
 void Class_DJI_Motor_GM6020::TIM_Alive_PeriodElapsedCallback()
/**
 * @brief TIM定时器中断计算回调函数
 *
 */
 void Class_DJI_Motor_C610::TIM_PID_PeriodElapsedCallback()

pid算法
P比例控制系数
I积分控制系数
D微分控制系数
