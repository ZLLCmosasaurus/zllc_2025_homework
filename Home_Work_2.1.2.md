# 学习记录

## Ⅰ.SPI

- **概念**：SPI是一种串口外设通信，主要通过主机(Master)和从机(Slave)之间互相发送数据的方式进行通信，一个主机可以连接多个从机。

![screen-capture](2849afd303eff29771c7828f002189b7.png)

***

- **基础配置**：SPI

主要分成非四线和四线两种模式，非SPI四线模式各引脚作用如图，

![screen-capture](690f29270de1c674f07b01b56ef97851.png)

一般来说不使用NSS而是额外配置一个IO口来选择拉高拉低电平，相对来说更灵活

SPI四线模式如图(仅了解，区别在于多了两个发送和接受的数据线，收发数据量更多)，

![screen-capture](23b0b51b985f38fc87303e73d06340e6.png)

SPI时序是由CKPH,CKPL决定的，CKPL的值为0/1指的是默认电平为低/高电平，CKPH的值为0/1指的是从奇数/偶数个边沿采样，当NSS被拉低时意味着寄存器开始接受数据

![screen-capture](0a647d662d6f078bcb1f17081de6d1a4.png)

***

在实验室infantry的CUBEMX配置中：

<br/>

![screen-capture](7017ae438f130b8aaba1bbf824f8931d.png)

选用的是全双工的主机模式，即主机发送和接收同时进行

![screen-capture](9d0670e5464cfe7477da6bebbb345b63.png)

选择自己配置IO口作为NSS

![screen-capture](98d8bfa5487dfc484adbb5725936a169.png)

这些配置需要查询对应从机元件的数据手册配置

- **DMA模式**：直接通过DMA搬运数据而不经过CPU的处理，从而节省CPU的处理时间

- 实验室代码框架

```c_cpp
//定义结构体
struct Struct_SPI_Manage_Object
{
    SPI_HandleTypeDef *SPI_Handler;
    GPIO_TypeDef* Now_GPIOx;
    uint16_t Now_GPIO_Pin;
    uint8_t Tx_Buffer[SPI_BUFFER_SIZE];
    uint8_t Rx_Buffer[SPI_BUFFER_SIZE];
    uint16_t Now_Tx_Length;
    uint16_t Now_Rx_Length;
    SPI_Call_Back Callback_Function;
};
//初始化
void SPI_Init(SPI_HandleTypeDef *hspi, SPI_Call_Back Callback_Function);
//用于交互收发数据帧
uint8_t SPI_Send_Receive_Data(SPI_HandleTypeDef *hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t Tx_Length, uint16_t Rx_Length)
/*嵌套使用了
HAL_SPI_TransmitReceive
HAL_SPI_TransmitReceive_DMA
*/
//SPI的TIM定时器中断交互回调函数，但未使用
void TIM_SPI_PeriodElapsedCallback()
//HAL库SPI交互DMA中断
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
```

<br/>

## Ⅱ.IMU和BMI088

- **概念**：BMI088 传感器是一个六轴惯性测量单元 (IMU)，能够应用于机器人上的姿态解算；陀螺仪是测量角速度的传感器，是 IMU 的重要组成部分。陀螺仪能测量在三个正交方向上旋转的角速度，也可以用于估算在三个方向上的旋转角度；六轴 IMU 中的另一重要组成部分是加速度计；顾名思义，加速度计能够测量三个正交方向上的加速度。MEMS 加速度计原理是利用加速度变化使内部质量块产生的力发生变化，从
而改变电容大小，转化为电信号。通信协议是用的SPI通信（抄的，如懂，实际上不懂）
- **代码框架**：（部分主要函数）

```c_cpp
//初始化配置，其中对BMI088传感器，IST8310传感器，MahonyAHRS算法，EKF初始化，温控pid参数都进行了初始化
void Class_IMU::Init()
//具体内容看不懂，但是里面有Get_Angle()函数，反正意思调用这个函数能获得最终的数据
void Class_IMU::TIM_Calculate_PeriodElapsedCallback(void)
//判断陀螺仪的状态
void Class_IMU::TIM1msMod50_Alive_PeriodElapsedCallback(void)
//被嵌套在其他函数
void Class_IMU::Get_Angle()
//也被嵌套在其他函数
void Class_IMU::TIM_Set_PWM(TIM_HandleTypeDef *tim_pwmHandle, uint8_t Channel, uint16_t value)
```