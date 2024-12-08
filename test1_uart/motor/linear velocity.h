#ifndef LINEAR_H
#define LINEAR_H

#include "math.h" 


#ifndef PI
#define PI 3.1415926
#endif

class FourWheelDrive 
{
private:
    float wheelbase;    
    float wheeltrack;   
    float max_speed;    

public:
    // 车长，轮距，最大速度
    FourWheelDrive(float L, float W, float max_v);

    // 四轮速度计算
    void computeWheelSpeeds(float omega, float linear_speed,  float &v_lf, float &v_rf, float &v_lr, float &v_rr);
};

#endif // FOURWHEELDRIVE_H
