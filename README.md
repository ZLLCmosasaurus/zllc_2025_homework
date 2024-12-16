- [嵌入式学习任务](#嵌入式学习任务)
  - [任务目标](#任务目标)
  - [任务点](#任务点)
    - [1. 学习git使用，学会使用git进行版本管理，学会使用github进行远程仓库管理，学会使用git进行团队协作。](#1-学习git使用学会使用git进行版本管理学会使用github进行远程仓库管理学会使用git进行团队协作)
    - [2.1 学习实验室仓库代码框架的基本使用](#21-学习实验室仓库代码框架的基本使用)
      - [2.1.1 学习 `CAN` 协议和大疆电机使用方法以及 `pid` 控制算法](#211-学习-can-协议和大疆电机使用方法以及-pid-控制算法)
      - [2.1.2 学习 `SPI` 通讯和 `BMI088` 陀螺仪的使用](#212-学习-spi-通讯和-bmi088-陀螺仪的使用)
      - [2.1.3 学习`裁判系统`通信协议的内容](#213-学习裁判系统通信协议的内容)
      - [2.1.4 学习 `UART` 通讯和 `Dbus` 接收机的使用](#214-学习-uart-通讯和-dbus-接收机的使用)
    - [2.2 学习实验室嵌入式开发的基本流程](#22-学习实验室嵌入式开发的基本流程)
      - [2.2.1 学习`Keil5` ，`vscode` 开发调试工具](#221-学习keil5-vscode-开发调试工具)
      - [2.2.2 学习调试软件`ozone`，`Vofa+`的使用](#222-学习调试软件ozonevofa的使用)
      - [2.2.3 学习`arm_none_eabi`的使用 （暂时与实验室无关，不要求掌握，有余力就去了解）](#223-学习arm_none_eabi的使用-暂时与实验室无关不要求掌握有余力就去了解)
  - [作业要求](#作业要求)

# 嵌入式学习任务

## 任务目标 
> 学习git使用，学习[实验室仓库](https://github.com/ZLLCmosasaurus/RMUC-2025)代码框架的基本使用，学习实验室嵌入式开发的基本流程，学习vscode，ozone等现代化开发调试工具的使用。  

## 任务点
### 1. 学习git使用，学会使用git进行版本管理，学会使用github进行远程仓库管理，学会使用git进行团队协作。 
> - [作业要求点这里](Home_Work/Home_Work_1.md)    
> - [git教程点这里](sources/git.md)  

### 2.1 学习[实验室仓库](https://github.com/ZLLCmosasaurus/RMUC-2025)代码框架的基本使用

  #### 2.1.1 学习 `CAN` 协议和大疆电机使用方法以及 `pid` 控制算法
  > 学习 `drv_can.cpp` 的使用方法，了解 `can` 总线的协议和使用方法。  
  > 学习 `dvc_djimotor.cpp` 的使用方法，了解大疆电机的通信协议和控制方法。  
  > 学习 `alg_pid.cpp` 的使用方法，了解 `pid` 调参的方法。  
  - 学习目标：完成对大疆电机pid速度和角度闭环的控制。
  - 学习记录：提交不少于一篇的学习记录，记录学习过程中遇到的问题和解决方法（md文档）。

  #### 2.1.2 学习 `SPI` 通讯和 `BMI088` 陀螺仪的使用
  > 学习 `drv_spi.cpp` 的使用方法，了解 `SPI` 的协议和 `DMA` 使用方法。  
  > 学习 `dvc_imu.cpp` 及其相关文件的使用方法，了解 `BMI088` 陀螺仪的通信协议和使用方法。  
  > 学习 `QuaternionEKF.c` 拓展卡尔曼滤波的使用方法，了解 `四元数` 和 `卡尔曼滤波` 的原理。（选做，不做要求，会用即可）   
  - 学习目标：完成对 `BMI088` 陀螺仪的数据接收和处理，结合 ` 2.1.1` 和 `2.1.2`  的代码，完成遥控器控制下，采用 `imu`  作为数据源，对大疆电机的 ` 速度`  和 ` 角度`  闭环控制。
  - 学习记录：提交不少于一篇的学习记录，记录学习过程中遇到的问题和解决方法（md文档）。
  
  #### 2.1.3 学习`裁判系统`通信协议的内容
  > 学习 `dvc_referee.cpp` 的使用方法，了解裁判系统的通信协议和解包的方法。  
  > 学习 `crc校验` 的原理和C语言实现，以及实验室仓库代码的使用。
  - 学习目标：完成对裁判系统的数据接收和处理。
  - 学习记录：提交不少于一篇的学习记录，记录学习过程中遇到的问题和解决方法（md文档）。

  #### 2.1.4 学习 `UART` 通讯和 `Dbus` 接收机的使用
  > 学习 `drv_uart.cpp` 的使用方法，了解串口的协议和DMA空闲中断使用方法。
  > 学习 `dvc_dr16.cpp` 的使用方法，了解dbus接收机的通信协议和控制方法。
  - 学习目标：完成对dbus接收机的数据接收和处理，了解 `Dbus` 串口的硬件原理。
  - 学习记录：提交不少于一篇的学习记录，记录学习过程中遇到的问题和解决方法（md文档）。

### 2.2 学习实验室嵌入式开发的基本流程

  #### 2.2.1 学习`Keil5` ，`vscode` 开发调试工具
  > 学习使用 `Keil5` 代码编写、编译、调试等流程。   
  > 学习 `vscode` 的使用，下载配置 `EIDE` 插件，完成 `vscode` 端的代码编写、编译、调试、上传等流程。[EIDE教程可以点这里](https://blog.csdn.net/m0_74858601/article/details/139050698)  
  - 学习目标：对实验室仓库代码的修改、编译、调试。
  - 学习记录：提交不少于一篇的学习记录，记录学习过程中遇到的问题和解决方法（md文档）。  
  #### 2.2.2 学习调试软件`ozone`，`Vofa+`的使用
  > 学习 `ozone` 的使用，使用 `ozone` 调试 `vscode` 端的代码，曲线的观测，数据的导出等等。 [ozone教程可以点这里](https://blog.csdn.net/NeoZng/article/details/127980949?spm=1001.2014.3001.5501)      
  > 学习 `Vofa+` 的使用，使用 `Vofa+` 串口调试 `vscode` 端的代码，曲线的观测，数据的导出等等。[vofa+下载可以点这里](https://www.vofa.plus/)   
  - 学习目标：对实验室仓库代码进行调试和曲线绘制。
  - 学习记录：提交不少于一篇的学习记录，记录学习过程中遇到的问题和解决方法（md文档）。 
  #### 2.2.3 学习`arm_none_eabi`的使用 （暂时与实验室无关，不要求掌握，有余力就去了解）
  > 学习 `arm_none_eabi` 编译环境的使用，使用 `gcc` 编译 `arm` 架构的代码,使用 `arm_none_eabi_gdb` 调试代码。（选做，不做要求）    
  > 学习使用 `cubmx` 配置 `Makefile` 工程，使用 `make` 编译代码。（选做，不做要求）
  


## 作业要求
> 提交方式：学习记录所提交的文件就按照第一次[作业的提交方式](sources/git.md)进行提交即可（本地修改，提交pr）。  
> 命名方式：在你自己名字命名的分支下创建名为Home_Work_2.1.x.md的文件(x对应不同的学习任务点)，将学习记录的内容写入该文件中。    
> 文件结构如下：
  ```
  分支名/
  ├── Keil 工程文件夹/
  │   ├── Core/
  │   ├── Drivers/
  │   ├── MDK-ARM/
  │   ├── ...
  |   |── ...
  ├── 图片等资源（小车的照片等）/
  ├── 其他文件（自己的学习日志等）/
  ├── Home_Work_2.1.1.md  <-- 新增的学习记录
  ├── Home_Work_2.1.2.md  <-- 新增的学习记录
  ├── Home_Work_2.1.x.md  <-- 新增的学习记录
  ├── .gitignore
  └── README.md
  ```
> 验收时间：每周组会进行验收。  
> 补充部分：所用的代码基于实验室仓库的main分支代码进行本地学习和修改，然后提交学习记录和完成学习要求，不要求大家从零开始写代码，只需要在实验室仓库的基础上进行学习和修改即可。（当然有能力的同学可以自己重写底层框架）
 
