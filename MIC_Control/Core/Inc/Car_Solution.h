#ifndef __CAR_SOLUTION_H
#define __CAR_SOLUTION_H


#ifdef __cplusplus
extern "C" {
#endif

void Getup_prveArm_1();
void Putdown_prveArm();
void Getup_backArm();
void Putdown_backArm();
void Servo_Init();
void Move_Transfrom(float Vx,float Vz);
void YaoKong_Move(int16_t X, int16_t Y);
void Getup_prveArm_2();

#ifdef __cplusplus
}
#endif

#endif