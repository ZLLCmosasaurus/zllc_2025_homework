#include "pid.h"



IncrementalPID::IncrementalPID(float p, float i, float d, float out_min, float out_max)
        : Kp(p), Ki(i), Kd(d), pre_error(0.0), pre_pre_error(0.0), output(0.0),
          output_min(out_min), output_max(out_max) {}

    // 计算增量式 PID 输出
 float IncrementalPID::calculate(float setpoint, float measured_value) {
        float error = setpoint - measured_value; // 当前误差

        // 增量计算
        float delta_output = Kp * (error - pre_error) +
                             Ki * error +
                             Kd * (error - 2 * pre_error + pre_pre_error);

        // 更新总输出
        output += delta_output;

        // 限制输出范围
        if (output > output_max) {
            output = output_max;
        } else if (output < output_min) {
            output = output_min;
        }

        // 更新误差值
        pre_pre_error = pre_error; // 更新前两次误差
        pre_error = error;         // 更新前一次误差

        return output;
    }

    // 调整 PID 参数
 void IncrementalPID::tuneParameters(float new_Kp, float new_Ki, float new_Kd) {
        Kp = new_Kp;
        Ki = new_Ki;
        Kd = new_Kd;
    }

