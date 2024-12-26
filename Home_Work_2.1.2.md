# Home_Work_2.1.2
## 1.学习了 drv_spi.cpp 的使用方法，了解 SPI 的协议和 DMA 使用方法。
### --SPI--
详细的SPI协议可以参考以下博客([SPI详解1](https://blog.csdn.net/Teminator_/article/details/141279117?ops_request_misc=%257B%2522request%255Fid%2522%253A%25226e6ac33b8b15ce6275ac55ff78d1350e%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=6e6ac33b8b15ce6275ac55ff78d1350e&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_click~default-2-141279117-null-null.142^v100^pc_search_result_base2&utm_term=spi%E5%8D%8F%E8%AE%AE&spm=1018.2226.3001.4187))([SPI详解2](https://blog.csdn.net/u010632165/article/details/109460814?ops_request_misc=%257B%2522request%255Fid%2522%253A%25226e6ac33b8b15ce6275ac55ff78d1350e%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=6e6ac33b8b15ce6275ac55ff78d1350e&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-109460814-null-null.142^v100^pc_search_result_base2&utm_term=spi%E5%8D%8F%E8%AE%AE&spm=1018.2226.3001.4187))。
需要注意SPI的初始化与CAN、UART相同都要在驱动层初始化。
### --DMA--
DMA的详解可以参考以下博客([DMA详解](https://blog.csdn.net/as480133937/article/details/104927922?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522cb4825a14f63c457676036cc0e92a880%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=cb4825a14f63c457676036cc0e92a880&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-104927922-null-null.142^v100^pc_search_result_base2&utm_term=DMA&spm=1018.2226.3001.4187))及keysking([keyskingDMA](https://www.bilibili.com/video/BV1AN41127VL?vd_source=495d3ee5b051d595195ffb24e9b1d155))的视频。
需要注意的是虽然DMA可以解决大量数据转移过度消耗CPU资源的问题，但其传输的数据量只能达到65535，并且当CPU和DMA同时访问相同的目标(RAM或外设)时，DMA请求可能会停止 CPU访问系统总线达若干个周期，总线仲裁器执行循环调度，可能会使CPU运行速度减缓。
## 2.学习了 dvc_imu.cpp 及其相关文件的使用方法，了解 BMI088 陀螺仪的通信协议和使用方法。
### --BMI088--
[BMI088数据手册](../zllc_2025_homework/FO/手册/C194919_姿态传感器-陀螺仪_BMI088_规格书_WJ339016.PDF)
[BMI088简介](https://blog.csdn.net/weixin_44080304/article/details/125065724?ops_request_misc=%257B%2522request%255Fid%2522%253A%25227e031c70466a8229348bcc843a898465%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=7e031c70466a8229348bcc843a898465&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-125065724-null-null.142^v100^pc_search_result_base2&utm_term=bmi088&spm=1018.2226.3001.4187)可以参考博客前半部分。
### --IMU--
[IMU简介](https://blog.csdn.net/weixin_51520821/article/details/135193432?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522c07deebc6ab0174cfd537da2f5307b98%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=c07deebc6ab0174cfd537da2f5307b98&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_click~default-2-135193432-null-null.142^v100^pc_search_result_base2&utm_term=IMU&spm=1018.2226.3001.4187)
[IMU详解](https://blog.csdn.net/QLeelq/article/details/112985306?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522c07deebc6ab0174cfd537da2f5307b98%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=c07deebc6ab0174cfd537da2f5307b98&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-112985306-null-null.142^v100^pc_search_result_base2&utm_term=IMU&spm=1018.2226.3001.4187)
[IMU函数建模](https://blog.csdn.net/u011341856/article/details/132948044?ops_request_misc=&request_id=&biz_id=102&utm_term=IMU&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-2-132948044.142^v100^pc_search_result_base2&spm=1018.2226.3001.4187)
## 3.实现了采用 imu 作为数据源，对大疆电机的角速度和角度闭环控制。
### **1.实例化对象
![](../zllc_2025_homework/pictures/2.1.2/01.png)
### **2.重写回调函数
![](../zllc_2025_homework/pictures/2.1.2/02.jpg)
![](../zllc_2025_homework/pictures/2.1.2/03.jpg)
![](../zllc_2025_homework/pictures/2.1.2/04.jpg)
找到
![](../zllc_2025_homework/pictures/2.1.2/05.jpg)
添加
![](../zllc_2025_homework/pictures/2.1.2/06.jpg)
### **3.初始化使能
![](../zllc_2025_homework/pictures/2.1.2/07.jpg)
![](../zllc_2025_homework/pictures/2.1.2/08.jpg)
### **4.接下来只需要做一个掉包侠就行
在void Task100us_TIM4_Callback()中调用
![](../zllc_2025_homework/pictures/2.1.2/09.jpg)
然后
![](../zllc_2025_homework/pictures/2.1.2/10.jpg)