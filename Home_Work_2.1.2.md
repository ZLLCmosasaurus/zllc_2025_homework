# 0.前置知识点  
## IMU和BMI088
IMU（惯性测量单元）和BMI088传感器在控制云台中具有重要作用。BMI088是一款高精度的六轴IMU，集成了三轴加速度计和三轴陀螺仪。它可以提供精确的角度、速度和加速度信息，帮助实现云台的平衡和稳定控制。  
### 解算过程  
在实际应用中，六轴IMU的主要任务是精确地估算物体的姿态（也就是物体的方向或倾斜角度）运动状态（加速度、速度（陀螺仪））   
假如我使用IMU来控制一架无人机飞行，IMU实质上是一个传感器，实时估算无人机的姿态，以保证它能够稳定飞行并执行特定的任务  
加速度  
加速度计用于测量加速度，尤其是**重力加速度**。当无人机静止时，IMU的加速度计能够感知重力的方向，并且根据重力的分布计算出无人机的倾斜角度。例如：

- 假设无人机的加速度计的三个轴的测量值分别是：
  - `ax`（X轴的加速度）
  - `ay`（Y轴的加速度）
  - `az`（Z轴的加速度）
x轴是pitch
y轴是roll
z轴是yaw
它相当于在不垂直的时候，可以看到知道哪个方向不垂直，但这样就只解决了一个问题，还有具体角度的求解，这样才能保持精度  
这样就需要一个陀螺仪了，这个传感器可以让我们知道角速度，角速度的积分就是角度，实际上通过双环PID控制  
这样得到的数据，通过加速度计和陀螺仪计可以得到now的数据，再和target就可以进入PID解算了
## SPI
SPI是一种高速、全双工、同步通信总线，高速：SPI没有定义速度限制，通常能达到甚至超过10M/bps；全双工：同时接收和发送；同步通信：同一个时钟源，主从设备共用一个，也正是因为这样，不需要像can通信一样有ID等  
SPI通信，需要至少4根线，单向传输时3根线，它们是MISO（主设备数据输入）、MOSI（主设备数据输出）、SCLK（时钟）和CS/SS（片选）：

MISO（ Master Input Slave Output）：主设备数据输入，从设备数据输出；
MOSI（Master Output Slave Input）：主设备数据输出，从设备数据输入；
SCLK（Serial Clock）：时钟信号，由主设备产生；
CS/SS（Chip Select/Slave Select）：从设备使能信号，由主设备控制，一主多从时，CS/SS是从芯片是否被主芯片选中的控制信号，只有片选信号为预先规定的使能信号时（高电位或低电位），主芯片对此从芯片的操作才有效。  
一个主设备可以接多个从设备
### 通信流程  
1. 主设备首先发起信号，将CS/SS拉低，启动通信  
2. 主设备通过发送时钟信号，来告诉从设备进行写数据或者读数据操作（采集时间可以通过位时序定？可采用上升沿或者下降沿通知从设备读取信息），从设备在得到信号后，它将立即读取数据线上的信号，这样就得到了一位数据（1bit）。  
3. 主机（Master）将要发送的数据写到发送数据缓存区（Menory），缓存区经过移位寄存器（缓存长度不一定，看单片机配置），串行移位寄存器通过MOSI信号线将字节一位一位的移出去传送给从机，同时MISO接口接收到的数据经过移位寄存器一位一位的移到接收缓存区。
4. 从机（Slave）也将自己的串行移位寄存器（缓存长度不一定，看单片机配置）中的内容通过MISO信号线返回给主机。同时通过MOSI信号线接收主机发送的数据，这样，两个移位寄存器中的内容就被交换。（3和4一起构成了全双工模式，以下是全双工实现）  
SPI是单主设备（Single Master）通信协议，只有一支主设备能发起通信，当SPI主设备想读/写从设备时，它首先拉低从设备对应的SS线（SS是低电平有效）。接着开始发送工作脉冲到时钟线上，在相应的脉冲时间上，主设备把信号发到MOSI实现“写”，同时可对MISO采样而实现“读”  
### 时钟配置（基础）  
SPI时钟特点主要包括：时钟速率、时钟极性和时钟相位三方面。

1. 时钟速率
SPI总线上的主设备必须在通信开始时候配置并生成相应的时钟信号。从理论上讲，只要实际可行，时钟速率就可以是你想要的任何速率，当然这个速率受限于每个系统能提供多大的系统时钟频率，以及最大的SPI传输速率。

2. 时钟极性
根据硬件制造商的命名规则不同，时钟极性通常写为CKP或CPOL。时钟极性和相位共同决定读取数据的方式，比如信号上升沿读取数据还是信号下降沿读取数据。

CKP可以配置为1或0。这意味着你可以根据需要将时钟的默认状态（IDLE）设置为高或低。极性反转可以通过简单的逻辑逆变器实现。你必须参考设备的数据手册才能正确设置CKP和CKE。

CKP = 0：时钟空闲IDLE为低电平 0；
CKP = 1：时钟空闲IDLE为高电平1。
3. 时钟相位
根据硬件制造商的不同，时钟相位通常写为CKE或CPHA。顾名思义，时钟相位/边沿，也就是采集数据时是在时钟信号的具体相位或者边沿；

CKE = 0：在时钟信号SCK的第一个跳变沿采样；
CKE = 1：在时钟信号SCK的第二个跳变沿采样。

# 1.代码解析  
先从tsk.cpp里面寻找
`chariot.Gimbal.Boardc_BMI.TIM_Calculate_PeriodElapsedCallback();`

跳转到这个函数当中，顾名思义，定时器的计算中断，这其实就到了解算部分了，不过是通过IMU(可能还有其它传感器)传输云台yaw、pitch、roll等数据给控制云台的电机，让电机精准控制云台，最终使得云台在赛场上能够如臂屈使，而IMU就是用来实时传输云台当前位置的传感器，得到的数据通过进一步的解算，给到PID
[IMU传感器基础知识](https://wenku.csdn.net/column/10769ka7cm)
```
 IMU_BMI088.BMI088_Read(&BMI088_Raw_Data);//这里好像就已经接收了SPI数据，接下来是底盘解算

    INS.Accel[0] = BMI088_Raw_Data.Accel[0];
    INS.Accel[1] = BMI088_Raw_Data.Accel[1];
    INS.Accel[2] = BMI088_Raw_Data.Accel[2];
    INS.Gyro[0] = BMI088_Raw_Data.Gyro[0];
    INS.Gyro[1] = BMI088_Raw_Data.Gyro[1];
    INS.Gyro[2] = BMI088_Raw_Data.Gyro[2];

void Class_IMU::Get_Angle()
{
    // 获取最终数据
    INS.Yaw = QEKF_INS.Yaw;
    INS.Pitch = QEKF_INS.Pitch;//为什么
    INS.Roll = QEKF_INS.Roll;
    INS.YawTotalAngle = QEKF_INS.YawTotalAngle;

    INS_Rad[0] = atan2f(2.0f*(INS_Quat[0]*INS_Quat[3]+INS_Quat[1]*INS_Quat[2]), 2.0f*(INS_Quat[0]*INS_Quat[0]+INS_Quat[1]*INS_Quat[1])-1.0f);
    INS_Rad[1] = asinf(-2.0f*(INS_Quat[1]*INS_Quat[3]-INS_Quat[0]*INS_Quat[2]));
    INS_Rad[2] = atan2f(2.0f*(INS_Quat[0]*INS_Quat[1]+INS_Quat[2]*INS_Quat[3]),2.0f*(INS_Quat[0]*INS_Quat[0]+INS_Quat[3]*INS_Quat[3])-1.0f);
    for(int i=0;i<3;i++){
        INS_Angle[i] = INS_Rad[i] * 180.0f / 3.1415926f;//角度值转换
    }
}
    float Class_IMU::Get_Angle_Roll(void)
{
    return (INS.Roll);
}

float Class_IMU::Get_Angle_Pitch(void)
{
    return (INS.Pitch);
}

float Class_IMU::Get_Angle_Yaw(void)
{
    return (INS.Yaw);
}

float Class_IMU::Get_Accel_X(void)
{
    return (INS.Accel[0]);
}
```
以上没有四元数的处理，总而言之，我们把BMI088_Raw_Data(IMU直接传输的数据)进行解算，得到我们需要的角度预期(需要角度及其双环的原因，自己查)

从以上的代码可以看到，把INS.Yaw等数据return给Get等函数，封装在Class_IMU这个类里

```
void Class_Gimbal_Yaw_Motor_GM6020::Transform_Angle()
{
    True_Rad_Yaw = IMU->Get_Rad_Yaw();//从IMU封装到Class_Gimbal封装，数据的传递
    True_Gyro_Yaw = IMU->Get_Gyro_Yaw(); 
    True_Angle_Yaw = IMU->Get_Angle_Yaw();   
}
```
在这个函数，我们把在云台那边的数据传到PID了，这个操作相当于将前线士兵调到后方来，接下来就是PID的解算了

PID解算的简单粗暴的理解是，输出一个target，再输出一个now，二者进行相减得到一个error，最后对error进行pid处理，得到一个out输出
现在我们有了now的数据(now的数据是目标的实时数据)，现在我们就只需要target数据，就可以进行PID操作了，那么在哪呢？
```
case (DJI_Motor_Control_Method_IMU_ANGLE)://双环控制
    {
        PID_Angle.Set_Target(Target_Angle);//这个是target的设置，是由上位机或者遥感操作，这个由uart传输
        if (IMU->Get_IMU_Status()!=IMU_Status_DISABLE)
        {
            //角度环
            PID_Angle.Set_Now(True_Angle_Yaw);
            PID_Angle.TIM_Adjust_PeriodElapsedCallback();

            Target_Omega_Radian = PID_Angle.Get_Out();

            //速度环
            PID_Omega.Set_Target(Target_Omega_Radian);
            PID_Omega.Set_Now(True_Gyro_Yaw);

        }
        else
        {
            //角度环
            PID_Angle.Set_Now(Data.Now_Angle);
            PID_Angle.TIM_Adjust_PeriodElapsedCallback();

            Target_Omega_Angle = PID_Angle.Get_Out();

            //速度环
            PID_Omega.Set_Target(Target_Omega_Angle);
            PID_Omega.Set_Now(Data.Now_Omega_Angle);

        }
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();
        Set_Out(PID_Omega.Get_Out());
    }
    break;
    Output();




```
从代码中，我们有了target和now，我们现在不管P、I、D参数，也不管它的操作逻辑，现在你只需要知道，你得到了一个数据，经过双环控制的pid，最终输出了一个可以直接传给电调的数据、可以控制电机角速度以达到目标值的数据，这个数据是out

```
void Class_DJI_Motor_GM6020::Output()
{
    CAN_Tx_Data[0] = (int16_t)Out >> 8;
    CAN_Tx_Data[1] = (int16_t)Out;
}
```
通过output函数，我们把out这个输出值，给到CAN_Tx_Data，这个其实通过指针，就把数据传给发送缓冲区，直接发送给电调的数据，总而言之，这个out数据最后给到了缓冲区