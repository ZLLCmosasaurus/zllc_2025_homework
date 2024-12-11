#include "encoder.h"
#include "pid.h"

void encoder_Init(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}


int Get_motorL_Encoder(int motor1_pulse)//读取左电机的编码器
{
    int motorL_Encoder;
    motorL_Encoder=(short)motor1_pulse;
    motor1_pulse=0;
    return motorL_Encoder;
}

int Get_motorR_Encoder(int motor2_pulse)//读取右电机的编码器
{
    int motorR_Encoder;
    motorR_Encoder=(short)motor2_pulse;
    motor2_pulse=0;
    return motorR_Encoder;
}

float speed_left_pid_control(int motor1_pulse)
{
    float cont_val=0;
    int32_t actual_speed;
    actual_speed = (float)(Get_motorL_Encoder(motor1_pulse)/(6.25*4*11*0.01));//当前的实际速度值
    cont_val = PID_speed_realize(&t_pid_speed_left, actual_speed);
    return cont_val;//输出能够实现期望速度值的需求速度值
}

float speed_right_pid_control(int motor2_pulse)
{
    float cont_val=0;
    int32_t actual_speed;
    actual_speed = (float)(Get_motorR_Encoder(motor2_pulse)/(6.25*4*11*0.01));//当前的实际速度值
    cont_val = PID_speed_realize(&t_pid_speed_right, actual_speed);
    return cont_val;//输出能够实现期望速度值的需求速度值
}

float speed_left_pid_pwm(int motor1_pulse)
{
    float speed_left_pid_pwm = speed_left_pid_control(motor1_pulse)*99.9;//需调试确定转速与PWM的对应关系
    return speed_left_pid_pwm;
}

float speed_right_pid_pwm(int motor2_pulse)
{
    float speed_right_pid_pwm = speed_right_pid_control(motor2_pulse)*99.9;//需调试确定转速与PWM的对应关系
    return speed_right_pid_pwm;
}
float motorL_Target_speed(int motor1_pulse,int Target_pwm)//目标速度的设定与获取*左
{
    PWMB(Target_pwm);
    Get_motorL_Encoder(motor1_pulse);
    float temp_val= (float)(Get_motorL_Encoder(motor1_pulse)/(6.25*4*11*0.01));
    set_pid_target(&t_pid_speed_left,temp_val);
    return get_pid_target(&t_pid_speed_left);
}

float motorR_Target_speed(int motor2_pulse,int Target_pwm)//目标速度的设定与获取*右
{
    PWMA(Target_pwm);
    Get_motorR_Encoder(motor2_pulse);
    float temp_val= (float)(Get_motorR_Encoder(motor2_pulse)/(6.25*4*11*0.01));
    set_pid_target(&t_pid_speed_right,temp_val);
    return get_pid_target(&t_pid_speed_right);
}

void motorControlL(uint8_t motor_state)
{
    switch (motor_state)
    {
    case 0:HAL_GPIO_WritePin(GPIOB,BIN2_Pin,GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOB,BIN1_Pin,GPIO_PIN_RESET);break; //停止
    case 1:HAL_GPIO_WritePin(GPIOB,BIN2_Pin,GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOB,BIN1_Pin,GPIO_PIN_RESET);break; //正转
    case 2:HAL_GPIO_WritePin(GPIOB,BIN2_Pin,GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOB,BIN1_Pin,GPIO_PIN_SET);break; //反转
    default:
        break;
    }
}

void motorControlR(uint8_t motor_state)
{
    switch (motor_state)
    {
    case 0:HAL_GPIO_WritePin(GPIOB,AIN2_Pin,GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOB,AIN1_Pin,GPIO_PIN_RESET);break; //停止
    case 1:HAL_GPIO_WritePin(GPIOB,AIN2_Pin,GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOB,AIN1_Pin,GPIO_PIN_RESET);break; //正转
    case 2:HAL_GPIO_WritePin(GPIOB,AIN2_Pin,GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOB,AIN1_Pin,GPIO_PIN_SET);break; //反转
    default:
        break;
    }
}

