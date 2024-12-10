#include "motor.h"
#include <math.h>
#include <tim.h>
#include "encode.h"
volatile int32_t encoder_count[4] = {0};    // 四个编码器的计数�??
volatile uint8_t last_state[4] = {0};      // 四个编码器的上一次状�??
float target_speed[4] = {0};               // 四个轮子的目标�?�度
Pose robot_pose = {0.0f, 0.0f, 0.0f};
PID_Controller pid[4]={0};

float wheel_circumference = 2 * M_PI * WHEEL_RADIUS;// 计算轮子周长
float Vmax=rpm/60*wheel_circumference;//最大线速度
float Wmax=Vmax/WHEEL_RADIUS;//最大角速度
float Vc=0;//目标线速度
float Wc=0;//目标角速度

//每个轮子的PID控制器：
void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->prev_error = 0;
    pid->integral = 0;
}

float PID_Compute(PID_Controller *pid, float target, float current)
{
    float error = target - current;
    pid->integral += error;
    float derivative = error - pid->prev_error;
    pid->prev_error = error;

    return pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
}
//差�?�运动学计算
//根据目标线�?�度和角速度，计算四个轮子的目标速度�??
void ComputeWheelSpeeds(float linear_velocity, float angular_velocity, float wheel_base)
{ 

    float left_speed = linear_velocity + angular_velocity * wheel_base / 2;
    float right_speed = linear_velocity - angular_velocity * wheel_base / 2;

    // 假设轮子顺序：左前�?�左后�?�右前�?�右后�??
    target_speed[0] = left_speed;
    target_speed[1] = left_speed;
    target_speed[2] = right_speed;
    target_speed[3] = right_speed;
}
//将PID输出转化为PWM信号控制电机�??
void SetMotorPWM(int motor_index, float pwm_value)
{


    if (pwm_value > 99.0f) pwm_value = 99.0f;
    if (pwm_value < 0.0f) pwm_value =0.0f ;

    // 假设使用TIM4的�?�道
    switch (motor_index) {
        case 0: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pwm_value); break;
        case 1: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, pwm_value); break;
        case 2: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pwm_value); break;
        case 3: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, pwm_value); break;
    }
}
float SET_MOTOR(int motor_index, float pwm_value){
                if(motor_index==0|motor_index==1) 
            {       //左
              if (pwm_value < 0.0f){
                 pwm_value = (-1)*pwm_value;
                  //反转
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
               }
             else {
                  //正转
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
               }
            }
            else {//右
                            if (pwm_value < 0.0f){
                 pwm_value = (-1)*pwm_value;
                  //反转
                 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
                 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
               }
             else {
                  //正转
                 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
                  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
               }
            }
						return pwm_value;
}


//jiesuan
/*
void DiffDriveKinematics(int32_t left_pulses, int32_t right_pulses) {
    // 脉冲数转位移
    float delta_s_l = (2.0f * M_PI * WHEEL_RADIUS / ENCODER_CPR) * left_pulses;
    float delta_s_r = (2.0f * M_PI * WHEEL_RADIUS / ENCODER_CPR) * right_pulses;

    // 计算底盘的线位移和角度变化
    float delta_s = (delta_s_l + delta_s_r) / 2.0f;
    float delta_theta = (delta_s_r - delta_s_l) / WHEEL_BASE;

    // 更新位姿
    robot_pose.x += delta_s * cos(robot_pose.theta);
    robot_pose.y += delta_s * sin(robot_pose.theta);
    robot_pose.theta += delta_theta;

    // 确保 theta 在 [-pi, pi] 范围内
    if (robot_pose.theta > M_PI) robot_pose.theta -= 2.0f * M_PI;
    if (robot_pose.theta < -M_PI) robot_pose.theta += 2.0f * M_PI;
}
*/
