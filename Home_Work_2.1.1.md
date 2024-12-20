(之前进行已经进行 PID 双环控制过了)
# 0 CAN
## 0.1 CAN 通信概述
- CAN（Controller Area Network）是一种多主机、全双工、无锁定的串行通信协议
- CAN 帧结构：
	- 标准帧（11位标识符）：数据帧的标识符为11位
	- 扩展帧（29位标识符）：数据帧的标识符为29位
	- 数据字段：数据字段包含0至8字节的数据
	- CRC校验：用于确保数据传输的完整性
	- ACK（确认位）：确认接收的数据
- 帧格式
	- ID（标识符）：帧的优先级，数值越小优先级越高
	- DLC（数据长度代码）：表示数据字段的长度（最大8字节）
	- RTR（远程请求帧）：标志帧是否为远程请求帧
	- 数据：要传输的实际数据
## 0.2 代码总览
```cpp
//定义缓冲区

//配置过滤器
void can_filter_mask_config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID){}

//初始化函数
//包含启动 CAN 函数,激活 CAN 中断
void CAN_Init(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function){}

//定时发送函数
uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length){}
void TIM_CAN_PeriodElapsedCallback(){}

//接收中断函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){}
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){}
```

## 0.3 dvc_djimotor文件
包含缓冲区的分配,
- 电机对应的 CAN 通信初始化,
- 电机数据处理,
- 用定时中断实现判断电机是否保持正常连接,进行 PID 控制

# 1 PID
- 比例控制 （P）
	- 根据当前误差计算控制输出
	- 增加比例增益 Kp会提高响应速度，但可能引入振荡
- 积分控制（I）
	- 根据误差的累积计算输出，用于消除稳态误差
- 微分控制（D）：
	- 根据误差变化率预测未来趋势，抑制快速变化的误差
	- 增加微分增益 Kd会改善系统稳定性，但可能对噪声敏感
- 总输出为三者之和

```cpp
PID_Init (){};
PID_Calculate(PID_t *, float, float){};
//可补充各项细节控制,如积分限制,输出限制
```

