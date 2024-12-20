一、基础知识

![8db19d28e28a9fcedd5c8a337075e7c9](pictures\8db19d28e28a9fcedd5c8a337075e7c9.png)



隐性电平（1）：电压差绝对值<2v

显性电平（0）：电压差绝对值>=2V



![50bd85dfae923828a9e318503f330a9b](pictures\50bd85dfae923828a9e318503f330a9b.png)

![QQ20241220-185031](pictures\QQ20241220-185031.png)

波特率：1Mbps

DATA[i]:      将输出扭矩电流数据存储为高八位和低八位两个数据，存储时用>>8右移拆分成两个数据，解算将高八位<<8左移

​                     8位与低八位合并成一个uint16_t类型数据。

PS:

C620电调红灯常亮原因：7Pin线与电机端子接触不良

![e688e756b17635ffcfc20538cefb7e5e](pictures\e688e756b17635ffcfc20538cefb7e5e.png)

C板调试接口引脚：

Pin1:SWDIO

Pin2:SWCLK

Pin3:GND

Pin4:VCC_3V





二、软件代码

1、初始化

总初始化	Task.Init();



a.CAN通信协议初始化

​	CAN_Init(hcan,callback);

callback指针指向函数 Chassis_Device_CAN1_Callback（），在其中调用接收中断使flag++，检测电机是否在线

b.定时器初始化

​	TIM_Init(htim,callback)

callback指针指向函数Task1ms_TIM5_Callback（），

​	调用chariot.TIM1msMod50_Alive_PeriodElapsedCallback()，激活chassis中的四个电机

​	调用TIM_CAN_PeriodElapsedCallback()，CAN通信发送数据

c.DWT初始化

​	DWT_Init（168）；

​	168为时钟树频率168Mhz

用于时间的读取，延时函数的构成，频率读取

d.交互层初始化

​	chariot.Init();

------Chassis.Init();                      底盘初始化

---------- Motor_Wheel[i].Init           电机初始

-----------Motor_Wheel[i].PID_Omega.Init(1200.0f, 30.0f, 0.01f, 32.0f, Motor_Wheel[i].Get_Output_Max(), Motor_Wheel[i].Get_Output_Max());              PID初始化



2.调用底盘解算

​	chariot.Chassis.convey(5.0,0.0,0.0,0.0);

​	 Motor_Wheel[i].Set_Target_Omega_Radian(M1_omega);将底盘解算后的数据传入PID

​      	 在TIM_PID_PeriodElapsedCallback调用PID计算函数

​		改变输出量out

​		传出输出量

​		Out = PID_Torque.Get_Out();

​		在Output()

​		{

​		  CAN_Tx_Data[0] = (int16_t)Out >> 8;

 		 CAN_Tx_Data[1] = (int16_t)Out;

​		}中赋值给传输数组，实现PID闭环控制

三、PID调参

![23c160b077b6284ac8da8d661e79dc04](pictures\23c160b077b6284ac8da8d661e79dc04.png)

​	K_P 比例系数

​	K_I 积分

​	K_D 微分

​	K_F 前馈
