#include "setpwm.h"
#include "tim.h"
#include "esp32.h"
extern float max_v[4]={1820.7202,1860.3321,1819.3321,1900.9454};
extern int pwm[4]={0};
extern int pwm_set[4]={0};
int pwm_test[4]={94,100,100,90};
int cal[4];
int setpwm(IncrementalPID pid,float SV,float PV,int i,usart_1* esp32)
{
		 cal[i]=pid.calculate(SV,PV);
	
   pwm[i]=100*cal[i]/max_v[i];
    
    if(pwm[i]>100)
    {
        pwm[i]=100;
    }
    if(pwm[i]<-100)
    {
        pwm[i]=-100;
    }
  
		if(esp32->LIFT)//ȫ��ǰ��
		{
      AIN(1);
      CIN(1);
      BIN(1);
      DIN(1);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pwm_test[0]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pwm_test[1]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,pwm_test[2]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,pwm_test[3]);			
		}
		else if(esp32->RIGHT)//��ת
		{
      AIN(1);
      CIN(1);
      BIN(-1);
      DIN(-1);

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,50);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,50);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,50);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,50);
		}
		else if(esp32->LEFT)//��ת
		{
       AIN(-1);
      CIN(-1);
      BIN(1);
      DIN(1);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,50);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,50);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,50);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,50);
		}
	
		else {
      if(pwm[i]>=0){
        pwm_set[i]=pwm[i];
        switch(i)
        {
            case 0:
            AIN(1);
            __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pwm_set[i]);
            break;
            case 1:
            BIN(1);
             __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pwm_set[i]);
            break;
            case 2:
            CIN(1);
             __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,pwm_set[i]);
            break;
            case 3:
            DIN(1);
             __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,pwm_set[i]);
            break;

		}		
      }else{
           pwm_set[i]=-pwm[i];
        switch(i)
        {
            case 0:
            AIN(-1);
            __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pwm_set[i]);
            break;
            case 1:
            BIN(-1);
            __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pwm_set[i]);
            break;
            case 2:
            CIN(-1);
            __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,pwm_set[i]);
            break;
            case 3:
            DIN(-1);
            __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,pwm_set[i]);
            break;

		}

		}
		
    }
    return pwm[i];
}
	

void steering(usart_1 *s)
{
	  if(s->FORWARD==1&&s->BACK==0)//向前抬腿
		{
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,50);
		}
		else if(s->FORWARD==0&&s->BACK==1)
		{	
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,180);
		}
		if(s->FORWARD==0&&s->BACK==0)//向后抬腿
		{
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,90);
		}

}

