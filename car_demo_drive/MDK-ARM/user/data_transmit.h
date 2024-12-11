#ifndef data_transmit_h
#define data_transmit_h

//变量声明
typedef struct {
    uint8_t button_status;  // 按键状态
    uint8_t joystick_x;     // 摇杆X轴
    uint8_t joystick_y;     // 摇杆Y轴
} ControlData;
extern ControlData control_data;
extern int16_t target_value[4];
//函数声明
void Update_PID_Target(ControlData control_data_xu);
#endif
