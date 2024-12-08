#include "stm32f1xx_hal.h"
#include "Motor.h"
#include "tim.h"

//W-> 轮距，机器人左右麦轮的距离，单位：m。
//H-> 轴距，机器人前后麦轮的距离，单位：m，
#define Car_H 1
#define Car_W 0.2 							//并没有一定要按车子实际来调
#define X_static 100
#define Y_static 100

int16_t Speed_YaoKong[4];

//ROS下整车移动量转换为单轮速度  x:前+后-  y:左+右-  z:逆+顺-
void Move_Transfrom(float Vx,float Vz)
{
	/*
	M1.Expect_Speed= 44.3736f * (float)(Vx-Vz*Car_W/2);                //将速度转换为 10ms内应捕获到的脉冲数
	M2.Expect_Speed= 44.3736f * (float)(Vx+Vz*Car_W/2);                //209*0.01*4/（0.03*2*3.14）
	M3.Expect_Speed= 44.3736f * (float)(Vx+Vz*Car_W/2);
  M4.Expect_Speed= 44.3736f * (float)(Vx-Vz*Car_W/2);
	*/

	Set_ExSpeed(&M1,44.3736f * (float)(Vx-Vz*Car_W/2));	
	Set_ExSpeed(&M2,44.3736f * (float)(Vx+Vz*Car_W/2));
	Set_ExSpeed(&M3,44.3736f * (float)(Vx+Vz*Car_W/2));
	Set_ExSpeed(&M4,44.3736f * (float)(Vx-Vz*Car_W/2));

	
  
} 


//遥感下，小车速度映射 
void YaoKong_Move(int16_t X, int16_t Y){
		X = X-100;
		if(Y > 130){
			Set_Speed(&M1,100);
			Set_Speed(&M2,-100);
			Set_Speed(&M3,-100);
			Set_Speed(&M4,100);
		}
		else if(Y < 70){
			Set_Speed(&M1,-100);
			Set_Speed(&M2,100);
			Set_Speed(&M3,100);
			Set_Speed(&M4,-100); 
		}
		else{
			Set_Speed(&M1,X*2);
			Set_Speed(&M2,X*2);
			Set_Speed(&M3,X*2);
			Set_Speed(&M4,X*2);
		}
}

//舵机PWM启动

//SEVRO2 == 1
//SEVRO1 == 4
//SEVRO3 == 2

void Servo_Init(){
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,250);
  //__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,50);      //初始-90度，后臂
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,50);

  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	
}

//前臂下降    设为目标180度，手动到了预期按键停止
void Putdown_prveArm(){
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,120);
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,180);
}

//前臂回收,回到半竖直状态
void Getup_prveArm_1(){
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,180);
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,120);
}

//后臂下降      初始位置为-90度
void Putdown_backArm(){
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,150);
}

//后臂回收      回收后为90度
void Getup_backArm(){
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,250);
}

//手臂恢复初始状态 ，竖直  
void Getup_prveArm_2(){
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,250);
  //__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,50);      //初始-90度，后臂
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,50);
}

       