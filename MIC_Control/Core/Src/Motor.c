#include "Motor.h"

// uint32_t Last_tick,N_tick;
struct Motor M1, M2, M3, M4;
struct Motor *M0;      // M0用于能在中断里更新Speed
uint8_t Now_Motor; // 记录当前操控测速的马达,用于在主程序中切换模拟开关

//电机初始状态（速度，转向）设置，4路PWM开启
// 10KHZ的PWM通道开启
void Motor_Init()
{
    Now_Motor = 1;
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);						//默认正转
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
	
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);           //PWM波Start不能用TIM_CHANNEL_ALL
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
    
}

//电机状态初始化，PID参数设置
// 对四个电机数据的初始化，  默认给正转的标志位flag=1
void Struct_Init()
{
    M1.Number = 1;
    M1.GPIOX_M = GPIOA;
    M1.GPIO_PIN_Motor = GPIO_PIN_5;
    M1.Kp = 0.758;
    M1.Ki = 1.180;
    M1.Kd = 0.278;
		M1.Add_PWM = 0;

	
    M2.Number = 2;
    M2.GPIOX_M = GPIOA;
    M2.GPIO_PIN_Motor = GPIO_PIN_12;
    M2.Kp = 1.1187;
    M2.Ki = 1.2586;
    M2.Kd = 0;
		M2.Add_PWM = 0;


    M3.Number = 3;
    M3.GPIOX_M = GPIOB;
    M3.GPIO_PIN_Motor = GPIO_PIN_15;
    M3.Kp = 0;
    M3.Ki = 0;
    M3.Kd = 0;
		M3.Add_PWM = 0;


    M4.Number = 4;
    M4.GPIOX_M = GPIOB;
    M4.GPIO_PIN_Motor = GPIO_PIN_14;
    M4.Kp = 0;
    M4.Ki = 0;
    M4.Kd = 0;
		M4.Add_PWM = 0;


}

// 设置电机转向，1正0反
// 正反转改变通过数据选择器实现
void Turn_Motor(struct Motor *M, uint8_t flag)
{
    if (flag == 1)
    {
        if(M == &M1 || M == &M4){
            HAL_GPIO_WritePin(M->GPIOX_M, M->GPIO_PIN_Motor, GPIO_PIN_RESET);
        }
        else{
            HAL_GPIO_WritePin(M->GPIOX_M, M->GPIO_PIN_Motor, GPIO_PIN_SET); 
        }
    }
    if (flag == 0)
    {
        if(M == &M1 || M == &M4){
            HAL_GPIO_WritePin(M->GPIOX_M, M->GPIO_PIN_Motor, GPIO_PIN_SET);
        }
        else{
            HAL_GPIO_WritePin(M->GPIOX_M, M->GPIO_PIN_Motor, GPIO_PIN_RESET);
        }
    }
}

//开启某个电机的测速
void Get_Speed(struct Motor *M)
{
    // 获取电机速度，根据编号Number来换模拟开关
    M0 = M;

    HAL_TIM_Base_Start_IT(&htim2);   // 开始定时器计数测速
    __HAL_TIM_SetCounter(&htim4, 0); // 先清空Encoder脉冲
    __HAL_TIM_SetCounter(&htim2, 0); // 清空定时器
}

// 设置单电机预期速度
void Set_ExSpeed(struct Motor *M, int16_t Expect_Speed)
{
    M->Expect_Speed = Expect_Speed;
}


// 设置单电机实际速度
void Set_Speed(struct Motor *M,int16_t PWM)
{   
    //采用的PWM模式2，而不是PWM模式1，因为用的两个上拉电阻，高阻态输出
		uint8_t flag = PWM > 0 ? 1 : 0;            // 设置电机正反转，根据实际改
    PWM = PWM > 0 ? PWM : -PWM; // abs(M->PWM*100)        0-200
    
		
		if(PWM <13) PWM = 0;			//计数器的偏差，速度在01波动时，Speed被滤波为0，
													//但实际上还会有很低速的转动，并且此时error一直为0，无法真正达到0m/s
		
	
		//只测两个电机，控四个轮子，1--3，2--4
    if (M->Number == 1)
    {
        Turn_Motor(M, flag);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, PWM);
				
    }
    else if (M->Number == 2)
    {
        Turn_Motor(M, flag);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, PWM);
				
			
    }
    else if (M->Number == 3)
    {
        Turn_Motor(M, flag);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, PWM);
    }
    else if (M->Number == 4)
    {
        Turn_Motor(M, flag);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, PWM);
    }	
		
}

//采用的PWM模式2，而不是PWM模式1，因为用的两个上拉电阻，高阻态输出,相当于取了两次反
//电机PID速度环的控制，增量式
void PID_Motor(struct Motor *M)
{
    M->error[0]   = M->Expect_Speed - M->Speed[3];
	
    M->Add_PWM       = M->Kp * (M->error[0] - M->error[1]) + M->Ki * M->error[0] +
                      M->Kd * (M->error[0] - 2 * M->error[1] + M->error[2]);
    M->PWM += M->Add_PWM;
		
		M->error[2]   = M->error[1];
    M->error[1]   = M->error[0];
		
		
    // 限幅
    if (M->PWM > 180) M->PWM = 180;
    if (M->PWM < -180) M->PWM = -180;
		
}

//对电机PID参数设置，方便调参
void PID_Set(float Kp, float Ki, float Kd){
	M1.Kp = Kp;
  M1.Ki = Ki;
  M1.Kd = Kd;
	
	M2.Kp = Kp;
  M2.Ki = Ki;
  M2.Kd = Kd;
	
	M3.Kp = Kp;
  M3.Ki = Ki;
  M3.Kd = Kd;
	
	M4.Kp = Kp;
  M4.Ki = Ki;
  M4.Kd = Kd;

}