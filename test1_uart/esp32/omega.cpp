#include "omega.h"

#include "esp32.h"

#include "math.h"

#define PI 3.1415926

// void angle_cal(float *angle,usart_1* an)
// {
// 	if (an->location_x == 0 &&an->location_y == 0) {
// 			*angle = PI/2; // ��ʼ�Ƕ�0
// 	} else {
// 			*angle = atan2((float)an->location_y, (float)an->location_x); // ʹ�� atan2 ��������Ƕ�
// 			if (*angle < 0) {
// 					*angle += 2 * PI; // �����Ƕ�
// 			}
// 	}
// // }
// float Get_Omega(float get_angle[2],float dt)
// {
// 	 return (get_angle[1]-get_angle[0])/dt; 
// }
