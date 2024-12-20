# 2.1.1

尝试梳理代码，学习如何驱动M2006电机。

理解 Task_Init()->CAN_Init(&hcan1, Gimbal_Device_CAN1_Callback)

​				UART_Init(&huart3, DR16_UART3_Callback, 18)

​				TIM_Init(&htim5, Task1ms_TIM5_Callback)



Task1ms_TIM5_Callback()->TIM_CAN_PeriodElapsedCallback()->CAN_Send_Data(&hcan1, 0x200, CAN1_0x200_Tx_Data, 8)

CAN_Tx_Data = allocate_tx_data(hcan, __CAN_ID)-> case (DJI_Motor_ID_0x203):

​    {

​      tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[4]);

​    }

等
阅读了M2006和C610的手册，尝试绕过遥控器操作

​				