# Home_Work_2.1.3

## 1、CRC校验

https://blog.csdn.net/u013073067/article/details/86621770

[CRC算法原理、推导及实现 - QIYUEXIN - 博客园](https://www.cnblogs.com/qiyuexin/p/18363214)

CRC校验本质上是选取一个合适的除数，要进行校验的数据是被除数，然后做模2除法，得到的余数就是CRC校验值。

下面用具体的例子做讲解：给定一组数据A：10110011（二进制），选取除数B：11001。

首先需要在被除数A后加4个比特位0（具体加几个0是根据除数B的位数决定的，比如这里B是5位，那么A后面加4个0；如果B选6位，则A后面加5个0，总之加的0的个数比除数B的个数少1位。后面还会提到怎么添加）。
进行模2除法运算。注意每次都是模2运算，即异或。
最后得到余数C就是CRC校验值。注意余数位数必须比除数少1位，如果不够前面加0补齐。运算如下图所示
            ![2.1.3_1](2.1.3_1.png)

## 

模 2 除法是一种在二进制系统中进行的除法运算，主要用于循环冗余校验码（CRC）的计算。它与普通的算术除法类似，但有一些关键的不同之处。

基本概念

模 2 除法的实质是异或运算。在模 2 除法中，每一位的除法结果不会影响其他位，即不需要向上一位借位。模 2 加法和模 2 减法的规则如下：

- 1 + 1 = 0
- 0 + 1 = 1
- 0 + 0 = 0
- 1 - 1 = 0
- 0 - 1 = 1
- 1 - 0 = 1

这些运算相当于二进制中的逻辑异或运算。

示例

以下是一个模 2 除法的具体示例：

假设我们有一个被除数 1111000 和一个除数 1101。我们可以按照以下步骤进行模 2 除法：

1. 将被除数和除数对齐，并进行异或运算。
2. 如果余数的首位是 1，则商为 1，否则商为 0。
3. 将余数右移一位，并继续进行异或运算，直到处理完所有位。

具体步骤如下：

1111000 ÷ 1101

步骤 1: 1111 ⊕ 1101 = 0010

步骤 2: 0100 ⊕ 1101 = 1001

步骤 3: 0010 ⊕ 1101 = 0111

最终得到的商为 110，余数为 1



## 代码

`Class_Referee::Data_Process` 函数的主要作用是对接收到的串口数据进行解析和处理。它从一个数据缓冲区中读取数据，检查数据帧的合法性（通过校验和验证），并根据帧头的命令 ID 解析不同类型的数据。



### 函数输入和初始化

```
buffer_index = 0; 
uint16_t buffer_index_max = UART_Manage_Object->Rx_Buffer_Length - 1;
```

- `buffer_index` 初始化为 0，用于跟踪当前缓冲区的读取位置。
- `buffer_index_max` 设置为 `Rx_Buffer_Length - 1`，表示数据缓冲区的最大索引位置。

### 进入主循环

```
while(buffer_index < buffer_index_max)
```

- 通过循环，逐字节地读取数据缓冲区（`UART_Manage_Object->Rx_Buffer`）中的数据，直到遍历整个缓冲区（从 `buffer_index` 到 `buffer_index_max`）。

### 验证帧头和校验和

```
if ((UART_Manage_Object->Rx_Buffer[buffer_index] == 0xA5) && 
    (Verify_CRC8_Check_Sum(&UART_Manage_Object->Rx_Buffer[buffer_index], 5) == 1))
```

- 通过检查当前字节是否为帧头（`0xA5`）来判断是否是有效的数据帧。
- 如果帧头为 `0xA5`，还需要通过 `Verify_CRC8_Check_Sum` 函数验证数据的校验和（`5` 字节长度）。只有校验和正确，才会继续处理数据。

### 提取命令 ID 和数据长度

```
cmd_id = (UART_Manage_Object->Rx_Buffer[buffer_index + 6]) & 0xff;
cmd_id = (cmd_id << 8) | UART_Manage_Object->Rx_Buffer[buffer_index + 5];  
data_length = UART_Manage_Object->Rx_Buffer[buffer_index + 2] & 0xff;
data_length = (data_length << 8) | UART_Manage_Object->Rx_Buffer[buffer_index + 1];
```

- `cmd_id` 是从数据帧中的第 5 和第 6 字节提取的 16 位命令 ID。
- `data_length` 是从第 1 和第 2 字节提取的数据长度。由于数据长度是 16 位，所以需要先合并两个字节。

### 解析数据类型并处理

根据 `cmd_id`（命令 ID）来选择对应的数据类型，并进行处理。每种命令对应一种数据结构，函数通过 `switch` 语句选择正确的命令类型并进行数据解析。

```
Enum_Referee_Command_ID CMD_ID = (Enum_Referee_Command_ID)cmd_id;
switch (CMD_ID)
```

- `CMD_ID` 是根据 `cmd_id` 转换成的枚举类型，表示不同的命令 ID。
- 通过 `switch` 语句，依据不同的 `CMD_ID` 来处理不同类型的命令数据。

### 处理每个命令

以 `Referee_Command_ID_GAME_STATUS` 为例：

```
case (Referee_Command_ID_GAME_STATUS):
{   
    if ((buffer_index_max - buffer_index >= (sizeof(Struct_Referee_Rx_Data_Game_Status) + 7)) &&
        Verify_CRC16_Check_Sum(&UART_Manage_Object->Rx_Buffer[buffer_index], sizeof(Struct_Referee_Rx_Data_Game_Status) + 7) == 1)
    {
        memcpy(&Game_Status, &UART_Manage_Object->Rx_Buffer[buffer_index + 7], sizeof(Struct_Referee_Rx_Data_Game_Status));
        buffer_index += sizeof(Struct_Referee_Rx_Data_Game_Status) + 7;
    }
}
break;
```

- 检查 `buffer_index_max` 是否足够容纳命令数据（`sizeof(Struct_Referee_Rx_Data_Game_Status) + 7` 字节）。
- 如果数据有效（通过 `Verify_CRC16_Check_Sum` 校验），将数据从缓冲区复制到对应的结构体（如 `Game_Status`）。
- 更新 `buffer_index`，指向下一段数据。

对于其他命令（如 `Referee_Command_ID_GAME_RESULT`, `Referee_Command_ID_GAME_ROBOT_HP` 等）也是类似的操作：

- 检查命令是否有效，数据长度是否合适，校验和是否正确。
- 如果正确，解析数据并更新 `buffer_index`。



### 处理逻辑

每个命令的处理逻辑大致是：

1. 检查当前命令是否包含足够的数据。
2. 验证该数据的校验和。
3. 如果校验通过，则将数据复制到对应的结构体中。
4. 更新 `buffer_index` 以继续处理后续数据。

例如：

- `Referee_Command_ID_EVENT_DATA` 命令会将数据存储到 `Event_Data` 中。
- `Referee_Command_ID_ROBOT_STATUS` 会将数据存储到 `Robot_Status` 中。

### 完成循环

```
buffer_index++;
```

- 每次处理完一个数据帧后，`buffer_index` 增加 1，继续检查下一个字节，直到遍历完整个数据缓冲区。

### 结束处理

在处理完所有数据帧后，函数结束。可以看到函数并未处理完整个缓冲区，它会随着命令帧的解析逐步更新 `buffer_index`，并跳过已经解析过的部分。

`Get_CRC8_Check_Sum` 函数的主要功能是计算给定数据的 CRC-8 校验值。这个过程遵循 CRC 算法的基本原理，通过逐字节处理数据，使用一个特定的生成多项式（通常会预先定义一个查找表）来计算校验码。



### `Get_CRC8_Check_Sum` 函数

### 

```
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8)
{
    unsigned char ucIndex;
    while (dwLength--)
    {
        ucIndex = ucCRC8 ^ (*pchMessage++);
        ucCRC8 = CRC8_TAB[ucIndex];
    }
    return(ucCRC8);
}
```

### 1. 输入参数

- `pchMessage`：指向待计算 CRC 校验和的消息数据的指针，消息内容是 `pchMessage` 指向的内存区域中的数据。
- `dwLength`：待计算数据的长度，表示数据的字节数。
- `ucCRC8`：CRC 的初始值：`0xFF`用于第一次计算时的起始校验。

### 2. 输出结果

- 返回值：函数返回计算出的 CRC-8 校验和，类型为 `unsigned char`，即一个 8 位无符号整数。

### 3. 函数的具体步骤

#### 步骤 1：初始化

```
unsigned char ucIndex;
```

- `ucIndex` 是一个临时变量，用来存储每次计算 CRC 值时的中间结果（即 CRC 的当前值与数据字节的异或结果）。

#### 步骤 2：循环处理每个字节

```
while (dwLength--)
```

- `dwLength` 是一个整型计数器，表示剩余数据字节数。每次循环会处理一个字节，直到数据的全部字节都处理完。每次循环 `dwLength` 会减 1，直到其值为 0。

#### 步骤 3：CRC-8 的异或操作

```
ucIndex = ucCRC8 ^ (*pchMessage++);
```

- `*pchMessage++` 获取当前字节的数据，并将 `pchMessage` 指针向后移动一个字节（`++` 是后缀递增，`pchMessage++` 会先获取指针指向的值，然后将指针增加，指向下一个字节）。
- `ucCRC8 ^ (*pchMessage++)` 将当前字节的值与当前 CRC 值进行异或（XOR）操作。这是 CRC 计算的核心步骤之一，通过异或操作可以逐步改变 CRC 的状态。
- 结果存储在 `ucIndex` 中，用于后续的查找操作。

#### 步骤 4：查找表更新 CRC

```
ucCRC8 = CRC8_TAB[ucIndex];
```

`CRC8_TAB`是一个事先定义好的查找表，表中存储了针对每个可能的索引值（`ucIndex`）的 CRC 值。通过查找表加速 CRC 计算过程。

- 查找表的大小通常为 256，因为 CRC-8 算法基于 8 位的异或操作，每个字节对应 256 个可能的索引（从 `0x00` 到 `0xFF`）。
- 查找表的内容是根据 CRC 8 位多项式预先计算出来的，每个值对应特定字节对 CRC 校验的影响。

- 通过 `CRC8_TAB[ucIndex]`，可以迅速获取异或操作后的 CRC 状态，而无需逐位计算。

#### 步骤 5：返回最终的 CRC 校验和

```
return(ucCRC8);
```

- 当所有字节都处理完成后，`ucCRC8` 存储的就是最终计算出的 CRC 校验和。该值就是函数的返回值。

### 4. 总结步骤

1. 初始化 CRC 值（由 `ucCRC8` 提供）。

2. 对输入数据的每一个字节进行处理：

   - 将当前字节与 CRC 值进行异或。
   - 使用查找表快速更新 CRC 值。

3. 返回最终的 CRC 校验和。

   ### 发送流程

![图片1](pic/%E5%9B%BE%E7%89%871.png)

## 2、裁判系统



## <img src="pic/%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE%202024-12-30%20194825.png" alt="屏幕截图 2024-12-30 194825" style="zoom:50%;" />

<img src="pic/%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE%202024-12-30%20195025.png" alt="屏幕截图 2024-12-30 195025" style="zoom:50%;" />
