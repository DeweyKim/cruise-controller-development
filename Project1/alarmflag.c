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

    if (veh_ready == false) {//���� Ready ����
        alarmflag = 0x00;
    }
    if (lgon == false) {//lgOn��ȣ Reset
        alarmflag = 0x01;
    }
    if (can_fault == true) {//Can Fault
        alarmflag = 0x02;
    }
    if (can_timeout == true) {//Can Timeout
        alarmflag = 0x03;
    }
    if (veh_speed < 50) { // ���� ���� 50km �̸��� ��� ��ư �Է� �޾��� ��
        if (cruise_button == true) {
            alarmflag = 0x04;
        }
    }

    return 0;
}