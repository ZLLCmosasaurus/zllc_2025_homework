#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif
 #include <stdint.h>
 // 底盘参数
#define WHEEL_RADIUS   34    // 轮子半径 (单位：mm)
#define WHEEL_BASE     23     // 左右两轮间距 (单位：mm)
#define ENCODER_CPR    11    // 编码�? CPR (每转脉冲�?)
#define GEAR_RATIO     30.0          // 减速比
#define M_PI           3.1415926536
#define rpm            330        //空载330，额定250

extern float Vmax;//最大线速度
extern float Wmax;//最大角速度
extern float Vc;//目标线速度
extern float Wc;//目标角速度
//extern volatile int32_t encoder_count[4];    // 四个编码器的计数�??（左前、左后、右前、右后）
//extern volatile uint8_t last_state[4];      // 四个编码器的上一次状�??
extern float target_speed[4];               // 四个轮子的目标�?�度
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float prev_error;
    float integral;
} PID_Controller;

extern PID_Controller pid[4];                     // 四个轮子的PID控制�??

// 位姿定义
typedef struct {
    float x;       // 机器人在全局坐标系中�? x 坐标
    float y;       // 机器人在全局坐标系中�? y 坐标
    float theta;   // 机器人朝�? (弧度)
} Pose;
extern Pose robot_pose ;


//每个轮子的PID控制器：
void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd);
float PID_Compute(PID_Controller *pid, float target, float current);
//差�?�运动学计算
//根据目标线�?�度和角速度，计算四个轮子的目标速度�??
void ComputeWheelSpeeds(float linear_velocity, float angular_velocity, float wheel_base);
//将PID输出转化为PWM信号控制电机�??
void SetMotorPWM(int motor_index, float pwm_value);
//jiesuan
void DiffDriveKinematics(int32_t left_pulses, int32_t right_pulses);
//设置正反转
float SET_MOTOR(int motor_index, float pwm_value);
#ifdef __cplusplus
}
#endif
 
#endif // MOTOR_H
/*
#ifndef MOTOR_H
#define MOTOR_H

extern volatile int32_t encoder_count[4] = {0};    // 四个编码器的计数�??
extern volatile uint8_t last_state[4] = {0};      // 四个编码器的上一次状�??
extern float target_speed[4] = {0};               // 四个轮子的目标�?�度
#endif
*/
