#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif


class IncrementalPID {
public:
    float Kp;                // 比例系数
    float Ki;                // 积分系数
    float Kd;                // 微分系数
    float pre_error;         // 上一次误差
    float pre_pre_error;     // 上上次误差
    float output;            // 当前输出值
    float output_min;        // 输出最小值
    float output_max;        // 输出最大值

    // 构造函数
    IncrementalPID(float p, float i, float d, float out_min, float out_max);

    // 计算增量式 PID 输出
    float calculate(float setpoint, float measured_value); 
    // 调整 PID 参数
    void tuneParameters(float new_Kp, float new_Ki, float new_Kd); 
};
#ifdef __cplusplus
}
#endif

#endif // PID_H
