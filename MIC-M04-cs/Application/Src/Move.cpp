#include "Motor.h"
#include "Move.h"

void Move_Front(int Speed[4])
{

    Motor_SetSpeed1(Speed[0]);
    Motor_SetSpeed2(Speed[1]);
    Motor_SetSpeed3(Speed[2]);
    Motor_SetSpeed4(Speed[3]);
}
void Move_Back(int Speed[4])
{
    Motor_SetSpeed1(-Speed[0]);
    Motor_SetSpeed2(-Speed[1]);
    Motor_SetSpeed3(-Speed[2]);
    Motor_SetSpeed4(-Speed[3]);
}
void Move_Right(int Speed[4])
{

    Motor_SetSpeed1(Speed[0]);
    Motor_SetSpeed2(-Speed[1]);
    Motor_SetSpeed3(-Speed[2]);
    Motor_SetSpeed4(Speed[3]);
}
void Move_Left(int Speed[4])
{
    Motor_SetSpeed1(-Speed[0]);
    Motor_SetSpeed2(Speed[1]);
    Motor_SetSpeed3(Speed[2]);
    Motor_SetSpeed4(-Speed[3]);
}
void Move_RTwist(int Speed[4])
{
    Motor_SetSpeed1(Speed[0]);
    Motor_SetSpeed2(-Speed[1]);
    Motor_SetSpeed3(Speed[2]);
    Motor_SetSpeed4(-Speed[3]);
}
void Move_LTwist(int Speed[4])
{
    Motor_SetSpeed1(-Speed[0]);
    Motor_SetSpeed2(Speed[1]);
    Motor_SetSpeed3(-Speed[2]);
    Motor_SetSpeed4(Speed[3]);
}
void Move_Stop(void)
{
    Motor_SetSpeed1(0);
    Motor_SetSpeed2(0);
    Motor_SetSpeed3(0);
    Motor_SetSpeed4(0);
}

