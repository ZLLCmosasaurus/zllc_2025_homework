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