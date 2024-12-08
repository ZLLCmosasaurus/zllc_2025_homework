// pid.h 文件
#ifndef PID_H
#define PID_H

class PID {
public:
    // 构造函数
    PID(float kp, float ki, float kd, float i_max, float out_max);

    // 计算函数
    float calculate(float ref, float fdb);

private:
    // 私有成员变量
    float kp;
    float ki;
    float kd;
    float i_max;
    float out_max;
    float ref;
    float fdb;
    float err[2];
    float p_out;
    float i_out;
    float d_out;
    float output;

    // 辅助函数
    void limitMinMax(float &value, float min, float max);
};

class PIDCascade {
public:
    // 构造函数
    PIDCascade(float kp_outer, float ki_outer, float kd_outer, float i_max_outer, float out_max_outer,
               float kp_inner, float ki_inner, float kd_inner, float i_max_inner, float out_max_inner);

    // 级联计算函数
    float cascadeCalculate(float angleRef, float angleFdb, float speedFdb);

private:
    // 私有成员变量
    PID outer;
    PID inner;
    float output;
};

#endif // PID_H