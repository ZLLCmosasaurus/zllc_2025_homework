# Home_Work_2.1.3
## 1.学习了 dvc_referee.cpp 的使用方法，了解了裁判系统的通信协议和解包的方法。
`UART_Init(&huart6, Referee_UART6_Callback, 128);`初始化了裁判系统
`void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length)`中可以看到初始化了不同的串口，判断是否为x串口数据，若是，将 UART 句柄、回调函数和接收缓冲区长度赋值给 `UARTx_Manage_Object `结构体的相应成员。然后使用 `HAL_UARTEx_ReceiveToIdle_DMA `函数启动基于 DMA 的空闲中断接收，最后禁用 USARTx 的 DMA 半传输中断（`__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);`）。
`Referee_UART6_Callback`调用`chariot.Referee.UART_RxCpltCallback(Buffer,Length);`，而在重写的`HAL_UARTEx_RxEventCallback`可以看到将`UART_Init`将`UARTx_Manage_Object `结构体的相应成员传参给`UARTx_Manage_Object.Callback_Function`,实际是传参给了`chariot.Referee.UART_RxCpltCallback`，并通过接收回调调用函数，进入` Data_Process();`
### Data_Process函数解析
`buffer_index = 0; `将缓冲区索引`buffer_index`初始化为 0，用于后续遍历接收缓冲区
`uint16_t buffer_index_max = UART_Manage_Object->Rx_Buffer_Length - 1;`获取接收缓冲区数据并处理，用于给数组赋变量
`while(buffer_index<buffer_index_max)`循环遍历缓冲区
`if ((UART_Manage_Object->Rx_Buffer[buffer_index]==0xA5) && 
    (Verify_CRC8_Check_Sum(&UART_Manage_Object->Rx_Buffer[buffer_index],5)==1))`首先检查当前缓冲区位置的数据是否以0xA5开头，然后调用Verify_CRC8_Check_Sum函数对从当前位置开始的 5 个字节数据进行 CRC8 校验和验证。如果校验通过，说明数据帧头和 CRC8 校验和正确，继续处理数据
`cmd_id=(UART_Manage_Object->Rx_Buffer[buffer_index+6])&0xff;
cmd_id=(cmd_id<<8)|UART_Manage_Object->Rx_Buffer[buffer_index+5];  `提取命令 ID
`data_length=UART_Manage_Object->Rx_Buffer[buffer_index+2]&0xff;
data_length=(data_length<<8)|UART_Manage_Object->Rx_Buffer[buffer_index+1];`提取数据长度
`Enum_Referee_Command_ID CMD_ID = (Enum_Referee_Command_ID)cmd_id;`将提取的命令 ID 转换为枚举类型Enum_Referee_Command_ID，以便后续使用`switch`语句进行更清晰的分支处理
之后每个`case`分支对应一个特定的命令 ID。以`Referee_Command_ID_GAME_STATUS`为例：
首先检查缓冲区剩余数据长度是否足够处理当前命令的数据，即`buffer_index_max - buffer_index >= sizeof(Struct_Referee_Rx_Data_Game_Status) + 7`。这里的7是帧头、CRC 校验和的长度。
然后调用`Verify_CRC16_Check_Sum`函数对从当前缓冲区位置开始的`sizeof(Struct_Referee_Rx_Data_Game_Status) + 7`字节数据进行 CRC16 校验和验证。
如果上述两个条件都满足，则使用`memcpy`函数将从`buffer_index + 7`位置开始的`sizeof(Struct_Referee_Rx_Data_Game_Status)`字节数据复制到`Game_Status`结构体中。
最后更新缓冲区索引`buffer_index`，使其指向下一个可能的帧头位置。
## 2.学习了 crc校验 的原理和C语言实现，以及实验室仓库代码的使用。
[CRC校验原理视频](https://www.bilibili.com/video/BV1V4411Z7VA?vd_source=495d3ee5b051d595195ffb24e9b1d155)
### (1). Get_CRC8_Check_Sum 函数
1.声明一个无符号字符型变量`ucIndex`，用于在计算过程中存储中间索引值。
2.通过`while`循环遍历数据，每次循环：
- 将当前的`ucCRC8`值与指针`pchMessage`所指的数据字节进行异或运算，结果存入`ucIndex`。
- 利用`ucIndex`作为索引，从`CRC8_TAB`查找表中获取新的`CRC8`值，更新 `ucCRC8`。
- 数据指针`pchMessage`指向下一个字节，数据长度`dwLength`减1。
3.循环结束后，返回最终计算得到的CRC8校验和。
### (2). Verify_CRC8_Check_Sum 函数
1.声明一个无符号字符型变量`ucExpected`并初始化为0，用于存储预期的CRC8校验和。
2.检查输入指针`pchMessage`是否为空，以及数据长度`dwLength`是否小于等于2（因为至少需要一个数据字节和一个校验和字节）。若满足条件，直接返回 0，表示验证失败。
3.调用`Get_CRC8_Check_Sum`函数，计算除最后一个字节（校验和字节）之外的数据的CRC8校验和，结果存入`ucExpected`。
4.将`ucExpected`与数据的最后一个字节（实际校验和字节）进行比较。若相等，返回 1，表示验证成功；否则返回 0，表示验证失败。
### (3). Append_CRC8_Check_Sum 函数
1.声明一个无符号字符变量`ucCRC`，并初始化为0，用于存储计算得到的 CRC8 校验和。
2.检查输入指针`pchMessage`是否为空，以及数据长度`dwLength`是否小于等于 2。如果满足条件，直接返回，不进行后续操作。
3.调用`Get_CRC8_Check_Sum`函数，计算除最后一个字节（预留用于存储校验和的位置）之外的数据部分的 CRC8 校验和，并将结果存储在`ucCRC`中。
4.将计算得到的 CRC8 校验和`ucCRC`赋值给数据序列的最后一个字节，完成 CRC8 校验和的追加操作。
### (4). Get_CRC16_Check_Sum 函数
1.检查输入指针`pchMessage`是否为空。若为空，直接返回0xFFFF。
声明一个无符号字符型变量`chData`，用于临时存储当前读取的数据字节。
2.通过`while`循环遍历数据，每次循环：
3.读取指针`pchMessage`所指的数据字节，存入`chData`，然后指针指向下一个字节。
4.根据 CRC16 计算规则更新`wCRC`值：将`wCRC`右移 8 位，然后与` CRC16_Table`中根据`wCRC`和`chData`计算得到的索引值对应的元素进行异或运算。
数据长度`dwLength`减 1。
循环结束后，返回最终计算得到的 CRC16 校验和。
### (5). Verify_CRC16_Check_Sum 函数
1.声明一个无符号 16 位整型变量`wExpected`并初始化为 0，用于存储预期的 CRC16 校验和。
检查输入指针`pchMessage`是否为空，以及数据长度`dwLength`是否小于等于 2（因为 CRC16 校验和占 2 个字节）。若满足条件，直接返回 0，表示验证失败。
3.调用`Get_CRC16_Check_Sum`函数，计算除最后两个字节（校验和字节）之外的数据的 CRC16 校验和，结果存入`wExpected`。
4.将`wExpected`的低 8 位与数据的倒数第二个字节比较，高 8 位与数据的最后一个字节比较。若都相等，返回 1，表示验证成功；否则返回 0，表示验证失败。
### (6). Append_CRC16_Check_Sum 函数
1.声明一个无符号 16 位整型变量`wCRC`并初始化为 0，用于存储计算得到的 CRC16 校验和。
2.检查输入指针`pchMessage`是否为空，以及数据长度`dwLength`是否小于等于 2。若满足条件，直接返回。
3.调用`Get_CRC16_Check_Sum`函数，计算除最后两个字节（预留用于存储校验和的位置）之外的数据的 CRC16 校验和，结果存入`wCRC`。
4.将`wCRC`的低 8 位赋值给数据的倒数第二个字节，高 8 位赋值给数据的最后一个字节，完成 CRC16 校验和的追加。