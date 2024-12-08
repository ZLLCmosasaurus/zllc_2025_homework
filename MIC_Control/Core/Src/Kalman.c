//一维卡尔曼滤波器，Pitch

//不进行状态转移，先验估计等于上一次最优估计

//Q为x的协方差矩阵
//R为z的协方差矩阵

float K,P_hat,Q = 0.0025,R = 1,P = 1,X = 0;
float A = 1, B = 0.002, H = 1;

float Kalman_fifter(float Speed){
    float Z;

    X = A * X;           						//先验估计X
    P_hat = P + Q;                  //先验估计协方差，A=1，省去
    Z = H*Speed;                //测量角度值Z
    K = P_hat/(P_hat + R);          //原本是矩阵运算，一阶矩阵退化为普通实数运算
    X = X + K*(Z-H*X);
    P = (1-K) * P_hat;

    return X;
}