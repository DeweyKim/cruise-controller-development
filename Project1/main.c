#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include "interface.h"
#include "button-interface.h"
#pragma warning(disable:4996)

#define MIN_SPD 50
#define MAX_SPD 200

#define true 1
#define false 0

typedef int int8_t;
typedef float float8_t;

typedef enum {
    CRUISE = 0,
    CANCEL,
    SET,
    RES
};

int8_t cruise_mode(int8_t current_spd, int8_t car_brake, int8_t cancel_button, int8_t cruise_button, int8_t is_fault);
static int8_t Accelerate(int8_t Short, int8_t Long);
static int8_t Decelerate(int8_t Short, int8_t Long);
//static int8_t readInput(int8_t value);
static int8_t alram_flag(int8_t veh_speed, int8_t veh_ready, int8_t lgon, int8_t can_fault, int8_t can_timeout, int8_t cruise_button);
void main();

int8_t current_spd=0;
static int8_t target_spd=0;
static int8_t extra_time=0;
static int8_t cruise_mode_status;

int8_t cruise_mode(int8_t spd, int8_t car_brake, int8_t cancel_button, int8_t cruise_button, int8_t is_fault) {
    int8_t mode = 0;
    if (is_fault == 1) {
        target_spd = -1;
        mode = 0;
    }

    else if ((cancel_button == 1) && (target_spd != -1)) {
        target_spd = -1;
        mode = 0;
    }

    else if ((car_brake == 1) && (target_spd != -1)) {
        mode = 0;
    }

    else if ((spd >= 50) && (cruise_button == 1)) {
        printf("Cruise Mode On\n");
        target_spd = spd;
        mode = 1;
    }

    else if (spd < 50) {
        printf("���ؼӵ� �̸��Դϴ�\n");
        mode = 0;
    }

    else
    {
        /* exception */
    }
    return mode;
}

static int8_t Accelerate(int8_t Short, int8_t Long) {
    if (Short == true) {
        target_spd += 1;
    }
    else if (Long == true) {
        target_spd += 5;
    }
    else
    {
        /* exception */
    }
    if (target_spd >= MAX_SPD) {
        target_spd = MAX_SPD;
    }
    return target_spd;
}

static int8_t Decelerate(int8_t Short, int8_t Long) {
    if (Short == true) {
        target_spd -= 1;
    }
    else if (Long == true) {
        target_spd -= 5;
    }
    else
    {
        /* exception */
    }
    if (target_spd < MIN_SPD) {
        target_spd = MIN_SPD;
    }

    return target_spd;
}
/*static int8_t readInput(int8_t value)
{
    printf("Input Current Speed : ");
    scanf("%d", &value);
    return value;
}*/
static int8_t alram_flag(int8_t veh_speed, int8_t veh_ready, int8_t lgon, int8_t can_fault, int8_t can_timeout, int8_t cruise_button)
{
    int8_t alarmflag;
    /*int8_t veh_speed = 0;
    int8_t veh_ready = true;
    int8_t lgon = true;
    int8_t can_fault = false;
    int8_t can_timeout = false;
    int8_t cruise_button = false;
    */

    if (veh_ready == false) {       //���� Ready ����
        alarmflag = 0x00;
        return alarmflag;
    }
    if (lgon == false) {            //lgOn��ȣ Reset
        alarmflag = 0x01;
        return alarmflag;
    }
    if (can_fault == true) {        //Can Fault
        alarmflag = 0x02;
        return alarmflag;
    }
    if (can_timeout == true) {      //Can Timeout
        alarmflag = 0x03;
        return alarmflag;
    }
    if (veh_speed < 50) {           // ���� ���� 50km �̸��� ��� ��ư �Է� �޾��� ��
        if (cruise_button == true) {
            alarmflag = 0x04;
            return alarmflag;
        }
    }
    return 0;
}
void main() {

    int8_t car_brake = 0;         //�극��ũ ���ۻ���
    int8_t cancel_btn = 0;        //Cancel��ư ���ۻ���
    int8_t cruise_btn = 0;        //ũ�����ư ���ۻ���
    int8_t is_fault = 0;          //Faultó��

    int8_t STATUS_KEY = 0; // �Է¹�ư ���� ��
    int8_t value = 0;
    int8_t press_time = 0;
    int8_t timer = 0;

    //current_spd = readInput(value);

    while(1) {  
        STATUS_KEY = GetCurrentInputKey();
        if (STATUS_KEY == InputKey_Cruise) {
            car_brake = 0;
            cancel_btn = 0;
            cruise_btn = 1;
            cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, cruise_btn, is_fault);
            if (cruise_mode_status == 0) {
                current_spd = readInput(value);
                target_spd = current_spd;
            }
        }
        else if ((STATUS_KEY == InputKey_SetAccel_Short) && (cruise_mode_status == 1)) {
             Accelerate(1, 0);
        }
        else if ((STATUS_KEY == InputKey_SetAccel_Long) && (cruise_mode_status == 1)) {
            Accelerate(0, 1);
        }
        else if ((STATUS_KEY == InputKey_ResDecel_Short) && (cruise_mode_status == 1)) {
            Decelerate(1, 0);
        }
        else if ((STATUS_KEY == InputKey_ResDecel_Long) && (cruise_mode_status == 1)) {
            Decelerate(0, 1);
        }
        else if (STATUS_KEY == InputKey_Brake) {
            if (cruise_mode_status == 1) {
                car_brake = 1;
                cancel_btn = 0;
                cruise_btn = 0;
                cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, cruise_btn, is_fault);
                printf("Cruise Mode Off�� ��ȯ�մϴ�.\n");
            }
            else {
                printf("�̹� Cruise Mode Off �����Դϴ�.\n");
            }
        }
        else if (STATUS_KEY == InputKey_Cancel) {
            if (cruise_mode_status == 1) {
                car_brake = 0;
                cancel_btn = 1;
                cruise_btn = 0;
                cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, cruise_btn, is_fault);
                printf("Cruise Mode Cancel�մϴ�.\n");
            }
            else {
                printf("�̹� Cruise Mode Off �����Դϴ�.\n");
            }
        }
        else if (STATUS_KEY == InputKey_Resume) {
            if (cruise_mode_status == 1) {
                printf("�̹� Cruise Mode �����Դϴ�.\n");
            }
            else {
                car_brake = 0;
                cancel_btn = 0;
                cruise_mode_status = 1;
                printf("��ǥ�ӵ�:%d\n", target_spd);
            }
        }
        else
        {
            break;
        }
        is_fault = alram_flag(current_spd, 1, 1, 0, 0, cruise_btn);
    }

}