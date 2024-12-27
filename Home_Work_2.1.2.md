# 0 SPI通讯
- Serial Peripheral Interface(串行外设接口)
- ==一主多从,4 引脚通讯,传输速度快==
- 数据在时钟信号上升沿采集,时钟信号须一直存在
- DMA 传输时,SPI 必须接受数据与发送数据配合运行

| 信号线  |                           |                         |
| ---- | ------------------------- | ----------------------- |
| SS   | 片选信号线                     | 通过引脚电平确定哪个从机通讯(通常低电平有效) |
| SCK  | 时钟信号线                     | 主设备产生                   |
| MOSI | Master Output Slave Input | 主机传送数据到从机               |
| MISO | Master Input Slave Output | 从机传送数据到主机               |
## 0.1 DMA
- Direct Memory Access(直接内存访问)
- ==绕过CPU直接读写系统内存,提高数据传输效率==

# 1 IMU
- Inertial Measurement Unit(惯性测量单元)
- BMI088陀螺仪 
	- 集成了三轴加速度计和三轴陀螺仪
## 1.1 MahonyAHRS
- Mahony Attitude and Heading Reference System(马霍尼姿态确定系统)
- 使用四元数来表示姿态
- 通过积分可以计算得到横滚、俯仰、航向增量
## 1.2 QuaternionEKF
- Quaternion Extended Kalman Filter(四元数扩展卡尔曼滤波器)
 - ==一种适用于处理旋转和姿态估计问题的非线性系统状态的算法==


(看的不是很懂)