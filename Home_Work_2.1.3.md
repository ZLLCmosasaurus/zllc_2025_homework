# 裁判系统

通信协议包括帧头、命令码ID、数据、帧尾部分

1.帧头部分

SOF为起始字节，在接受裁判系统的数据时首先需要进行判断帧头部分的起始字节是否正确；因为数据的长度是不确定的，也就是不定长的，所以需要data_length来告知即将接收到的数据长度；seq表示这个数据包的序号；CRC8检验位用来确保数据的准确性。

```c_cpp
/**
 * @brief 裁判系统源数据
 *
 */
struct Struct_Referee_UART_Data
{
    uint8_t Frame_Header;
    uint16_t Data_Length;
    uint8_t Sequence;
    uint8_t CRC_8;
    Enum_Referee_Command_ID Referee_Command_ID;
    uint8_t Data[121];
} __attribute__((packed));
```

2.命令码ID+数据

裁判系统发送的数据是繁多的，同时发送所有信息会导致信息处理延迟较高，通过命令码，可以让机器人处理数据的速度加快，不需要遍历所有数据后分段处理，可以直接对不同命令码的数据包直接进行处理。每个命令码ID对应的数据包也是不一样的，主要通过结构体定义来类别。（如实验室代码用枚举类型和结构体定义对数据进行封装和处理）

```c_cpp
//示例代码
/**
 * @brief 裁判警告等级
 *
 */
enum Enum_Referee_Data_Event_Referee_Warning_Level : uint8_t
{
    Referee_Data_Referee_Warning_Level_Both_YELLOW= 1,//双方黄牌
    Referee_Data_Referee_Warning_Level_YELLOW,
    Referee_Data_Referee_Warning_Level_RED,
    Referee_Data_Referee_Warning_Level_FAIL,
};
/**
 * @brief 裁判系统经过处理的数据, 0x0001比赛状态, 3Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Game_Status
{
    uint8_t Type_Enum : 4;
    uint8_t Stage_Enum : 4;
    uint16_t Remaining_Time;
    uint64_t Timestamp;
    uint16_t CRC_16;
} __attribute__((packed));
```

3.帧尾

主要是进行CRC校验

实验室代码

```c_cpp
//首先对裁判系统进行初始化，选定UART进行接收
void Class_Referee::Init(UART_HandleTypeDef *huart, uint8_t __Frame_Header)
//在这个函数中进行数据处理，先进行校验和帧头判断，在对命令码ID进行判断处理
void Class_Referee::Data_Process()
//UART通信接收回调函数
void Class_Referee::UART_RxCpltCallback(uint8_t *Rx_Data,uint16_t Length)
//TIM定时器中断定期检测裁判系统是否存活
void Class_Referee::TIM1msMod50_Alive_PeriodElapsedCallback()
```

# CRC校验

1.奇偶检验【CRC 校验的一种(CRC-1)】

通讯过程的校验是指在通讯数据后加上一些附加信息，通过这些附加信息来判断接收到的数据是否和发送出的数据相同。奇偶校验就是在发送的每一个字节后都加上一位，使得每个字节中1的个数为奇数个或偶数个。

比如我们要发送的字节是0x1a，二进制表示为0001 1010。采用奇校验，则在数据后补上个0，数据变为0001 1010 0，数据中1的个数为奇数个（3个）采用偶校验，则在数据后补上个1，数据变为0001 1010 1，数据中1的个数为偶数个（4个）

接收方通过计算数据中1个数是否满足奇偶性来确定数据是否有错。

2.累加和校验【CRC校验在传输数据的形式上与累加和校验是相同的】

所谓累加和校验实现方式有很多种，最常用的一种是在一次通讯数据包的最后加入一个字节的校验数据。这个字节内容为前面数据包中全部数据的忽略进位的按字节累加和。

比如下面的例子：我们要传输的信息为： 6、23、4加上校验和后的数据包：6、23、4、33这里 33 为前三个字节的校验和。

接收方收到全部数据后对前三个数据进行同样的累加计算，如果累加和与最后一个字节相同的话就认为传输的数据没有错误。

3.C语言实现

(1)设置CRC寄存器，并给其赋值为“余数初始值”。

(2)将数据的第一个8-bit字符与CRC寄存器进行异或，并把结果存入CRC寄存器。

(3)CRC寄存器向右移一位，MSB补零，移出并检查LSB。

(4)如果LSB为0，重复第三步；若LSB为1，CRC寄存器与0x31相异或。

(5)重复第3与第4步直到8次移位全部完成。此时一个8-bit数据处理完毕。

(6)重复第2至第5步直到所有数据全部处理完成

(7) 最终CRC寄存器的内容与“结果异或值”进行或非操作后即为CRC值。

```c_cpp
#define POLY        0x1021
/**
 * Calculating CRC-16 in 'C'
 * @para addr, start of data
 * @para num, length of data
 * @para crc, incoming CRC
 */
uint16_t crc16(unsigned char *addr, int num, uint16_t crc)
{
    int i;
    for (; num > 0; num--)              /* Step through bytes in memory */
    {
        crc = crc ^ (*addr++ << 8);     /* Fetch byte from memory, XOR into CRC top byte*/
        for (i = 0; i < 8; i++)             /* Prepare to rotate 8 bits */
        {
            if (crc & 0x8000)            /* b15 is set... */
                crc = (crc << 1) ^ POLY;    /* rotate and XOR with polynomic */
            else                          /* b15 is clear... */
                crc <<= 1;                  /* just rotate */
        }                             /* Loop for 8 bits */
        crc &= 0xFFFF;                  /* Ensure CRC remains 16-bit value */
    }                               /* Loop until num=0 */
    return(crc);                    /* Return updated CRC */
}
```

<br/>

实验室代码（摘自referee.cpp)

CRC8校验

```c_cpp
unsigned char Get_CRC8_Check_Sum(unsigned  char  *pchMessage,unsigned  int dwLength,unsigned char ucCRC8)
{
	unsigned char ucIndex;
	while (dwLength--)
	{
	ucIndex = ucCRC8^(*pchMessage++);
	ucCRC8 = CRC8_TAB[ucIndex];
	}
	return(ucCRC8);
}
```

这个函数的主要功能是计算给定数据（ *pchMessage ）的CRC8校验和。它通过循环遍历数据的每个字节，与当前的CRC8值进行异或操作，并根据异或结果从 CRC8_TAB 数组中获取新的CRC8值，直到所有数据字节都被处理完。最后，函数返回计算得到的CRC8校验和。

```c_cpp
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength)
{
	unsigned char ucExpected = 0;
	if ((pchMessage == 0) || (dwLength <= 2)) return 0;
	ucExpected = Get_CRC8_Check_Sum (pchMessage, dwLength-1, CRC8_INIT);
	return ( ucExpected == pchMessage[dwLength-1] );
}
```

这个函数的主要功能是验证给定数据（ *pchMessage ）的CRC8校验和。它首先检查输入参数的合法性，然后调用 Get_CRC8_Check_Sum 函数计算数据的CRC8校验和，并将结果与数据的最后一个字节进行比较，以确定校验和是否正确。

```c_cpp
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength)
{
	unsigned char ucCRC = 0;
	if ((pchMessage == 0) || (dwLength <= 2)) return;
	ucCRC = Get_CRC8_Check_Sum ( (unsigned char *)pchMessage, dwLength-1, CRC8_INIT);
	pchMessage[dwLength-1] = ucCRC;
}
```

这个函数的主要功能是为给定的消息（ *pchMessage ）添加CRC8校验和。它首先检查输入参数的合法性，然后调用 Get_CRC8_Check_Sum 函数计算消息的CRC8校验和，并将校验和添加到消息的末尾

CRC16校验

```c_cpp
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC)
{
uint8_t chData;
	if (pchMessage == NULL)
	{
		return 0xFFFF;
	}
	while(dwLength--)
	{
		chData = *pchMessage++;
		(wCRC) = ((uint16_t)(wCRC) >> 8) ^ CRC16_Table[((uint16_t)(wCRC)^(uint16_t)(chData)) & 0x00ff];
	}
	return wCRC;
}
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength)
{
	uint16_t wExpected = 0;
	if ((pchMessage == NULL) || (dwLength <= 2))
	{
	return 0;
	}
	wExpected = Get_CRC16_Check_Sum ( pchMessage, dwLength - 2, CRC16_INIT);
	return ((wExpected & 0xff) == pchMessage[dwLength - 2] && ((wExpected >> 8) & 0xff) ==
	pchMessage[dwLength - 1]);
}
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength)
{
	uint16_t wCRC = 0;
	if ((pchMessage == NULL) || (dwLength <= 2))
	{
	return;
	}
	wCRC = Get_CRC16_Check_Sum ( (uint8_t *)pchMessage, dwLength-2, CRC16_INIT );
	pchMessage[dwLength-2] = (uint8_t)(wCRC & 0x00ff);
	pchMessage[dwLength-1] = (uint8_t)((wCRC >> 8)& 0x00ff);
}
```

各个函数的功能与CRC8类同，主要是函数内部对数据的处理方式不同
