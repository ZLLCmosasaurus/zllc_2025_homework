// pid.cpp 文件
#include "pid.h"

// PID 构造函数
PID::PID(float kp, float ki, float kd, float i_max, float out_max)
    : kp(kp), ki(ki), kd(kd), i_max(i_max), out_max(out_max), ref(0.0f), fdb(0.0f),
      p_out(0.0f), i_out(0.0f), d_out(0.0f), output(0.0f) {
    err[0] = 0.0f;
    err[1] = 0.0f;
}

// 限制输出范围函数
void PID::limitMinMax(float &value, float min, float max) {
    if (value > max) value = max;
    else if (value < min) value = min;
}

// 计算函数
float PID::calculate(float ref, float fdb) {
    this->ref = ref;
    this->fdb = fdb;

    // 更新误差
    err[1] = err[0];
    err[0] = ref - fdb;

    // 计算P输出
    p_out = kp * err[0];

    // 计算I输出并限制范围
    i_out += ki * err[0];
    limitMinMax(i_out, -i_max, i_max);

    // 计算D输出，采用简单低通滤波
    float d_error = err[0] - err[1];
    float alpha = 0.1f;  // 微分平滑系数
    d_out = alpha * (kd * d_error) + (1 - alpha) * d_out;

    // 计算总输出
    output = p_out + i_out + d_out;
    limitMinMax(output, -out_max, out_max);

    // 反积分饱和：当输出受限时，调整积分项
    if ((output == out_max && err[0] > 0) || (output == -out_max && err[0] < 0)) {
        i_out -= ki * err[0];
    }

    return output;
}

// PIDCascade 构造函数
PIDCascade::PIDCascade(float kp_outer, float ki_outer, float kd_outer, float i_max_outer, float out_max_outer,
                       float kp_inner, float ki_inner, float kd_inner, float i_max_inner, float out_max_inner)
    : outer(kp_outer, ki_outer, kd_outer, i_max_outer, out_max_outer),
      inner(kp_inner, ki_inner, kd_inner, i_max_inner, out_max_inner),
      output(0.0f) {}

// 级联计算函数
float PIDCascade::cascadeCalculate(float angleRef, float angleFdb, float speedFdb) {
    // 计算外层PID
    float outerOutput = outer.calculate(angleRef, angleFdb);
    
    // 内层PID以外层输出为参考值
    float innerOutput = inner.calculate(outerOutput, speedFdb);
    
    // 内层输出作为总输出
    output = innerOutput;

    return output;
}