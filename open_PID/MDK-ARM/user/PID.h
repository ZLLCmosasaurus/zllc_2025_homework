#ifndef PID_H
#define PID_H
typedef struct  {
    float Kp;
    float Ki;
    float Kd;
    float prev_error;
    float integral;
} PID;
extern PID motor_pid[4];
int16_t PID_Compute(PID* motor_pid, int16_t target_value, int16_t actual_value);
#endif
