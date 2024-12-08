#include <algorithm> // std::max 和 std::min

#include <cmath>    // std::tan
#include "linear velocity.h"


// 构造函数
FourWheelDrive::FourWheelDrive(float L, float W, float max_v)
    : wheelbase(L), wheeltrack(W), max_speed(max_v) {}

// 计算轮子的线速度
void FourWheelDrive::computeWheelSpeeds
(float omega, float linear_speed, float &v_lf, float &v_rf, float &v_lr, float &v_rr) 
{
    //Omega为正时，小车转动为正方向

    
    // 计算转弯半径
    
    float R=(sqrt(wheelbase*wheelbase+wheeltrack*wheeltrack))/2;
    
    //计算cos0
    
    float cos0=wheeltrack/(2*R);

    float sin0=wheelbase/(2*R);
    
    if (omega == 0) {
        
        // 直行时四个轮子的速度相同
       
        v_lf = v_rf = v_lr = v_rr = linear_speed;
    
    } else {
        // 计算每个轮子的线速度
        v_lf = linear_speed+omega*R*cos0;

        v_rf = linear_speed-omega*R*sin0;
        
        v_lr = linear_speed+omega*R*cos0;
        
        v_rr = linear_speed-omega*R*sin0;

        // 限制速度在最大速度范围内
        
        v_lf = std::max(std::min(v_lf, max_speed), -max_speed);
        
        v_rf = std::max(std::min(v_rf, max_speed), -max_speed);
        
        v_lr = std::max(std::min(v_lr, max_speed), -max_speed);
        
        v_rr = std::max(std::min(v_rr, max_speed), -max_speed);
    }
}
