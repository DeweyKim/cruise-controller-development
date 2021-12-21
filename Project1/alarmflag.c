#include <stdio.h>
#include<stdbool.h>

int main() {

    int alarmflag;
    int veh_speed = 0;
    bool veh_ready = true;
    bool lgon = true;
    bool can_fault = false;
    bool can_timeout = false;
    bool cruise_button = false;

    if (veh_ready == false) {//차량 Ready 해제
        alarmflag = 0x00;
    }
    if (lgon == false) {//lgOn신호 Reset
        alarmflag = 0x01;
    }
    if (can_fault == true) {//Can Fault
        alarmflag = 0x02;
    }
    if (can_timeout == true) {//Can Timeout
        alarmflag = 0x03;
    }
    if (veh_speed < 50) { // 현재 차속 50km 미만인 경우 버튼 입력 받았을 때
        if (cruise_button == true) {
            alarmflag = 0x04;
        }
    }

    return 0;
}