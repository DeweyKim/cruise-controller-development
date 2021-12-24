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

typedef enum {
    CRUISE = 0,
    CANCEL,
    SET,
    RES
};

int8_t cruise_mode(int8_t spd, int8_t car_brake, int8_t cancel_button, int8_t cruise_button, int8_t is_fault);
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
        printf("기준속도 미만입니다\n");
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
static int8_t alram_flag(int8_t veh_speed, int8_t veh_ready, int8_t lgon, int8_t can_fault, int8_t can_timeout, int8_t cruise_button)
{
    int8_t alarmflag = defAlarmFlag_invalid;

    if (veh_ready == false) {       //차량 Ready 해제
        alarmflag |= defAlarmFlag_vehReady;
    }
    if (lgon == false) {            //lgOn신호 Reset
        alarmflag |= defAlarmFlag_IgOn;
    }
    if (can_fault == true) {        //Can Fault
        alarmflag |= defAlarmFlag_canFault;
    }
    if (can_timeout == true) {      //Can Timeout
        alarmflag |= defAlarmFlag_canTimeout;
    }
    if (veh_speed < 50) {           // 현재 차속 50km 미만인 경우 버튼 입력 받았을 때
        if (cruise_button == true) {
            alarmflag |= defAlarmFlag_vehSpeed;
        }
    }
    return 0;
}
void main() {

    int8_t car_brake = 0;         //브레이크 동작상태
    int8_t cancel_btn = 0;        //Cancel버튼 동작상태
    int8_t cruise_btn = 0;        //크루즈버튼 동작상태
    int8_t is_fault = 0;          //Fault처리

    int8_t STATUS_KEY = 0; // 입력버튼 상태 값
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
                STATUS_KEY = GetCurrentInputKey();
            }
        }
        else if ((STATUS_KEY == InputKey_SetAccel_Short) && (cruise_mode_status == 1)) {
            target_spd = Accelerate(1, 0);
        }
        else if ((STATUS_KEY == InputKey_SetAccel_Long) && (cruise_mode_status == 1)) {
            target_spd = Accelerate(0, 1);
        }
        else if ((STATUS_KEY == InputKey_ResDecel_Short) && (cruise_mode_status == 1)) {
            target_spd = Decelerate(1, 0);
        }
        else if ((STATUS_KEY == InputKey_ResDecel_Long) && (cruise_mode_status == 1)) {
            target_spd = Decelerate(0, 1);
        }
        else if (STATUS_KEY == InputKey_Brake) {
            if (cruise_mode_status == 1) {
                car_brake = 1;
                cancel_btn = 0;
                cruise_btn = 0;
                cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, cruise_btn, is_fault);
                printf("Cruise Mode Off로 변환합니다.\n");
            }
            else {
                printf("이미 Cruise Mode Off 상태입니다.\n");
            }
        }
        else if (STATUS_KEY == InputKey_Cancel) {
            if (cruise_mode_status == 1) {
                car_brake = 0;
                cancel_btn = 1;
                cruise_btn = 0;
                cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, cruise_btn, is_fault);
                printf("Cruise Mode Cancel합니다.\n");
            }
            else {
                printf("이미 Cruise Mode Off 상태입니다.\n");
            }
        }
        else if (STATUS_KEY == InputKey_Resume) {
            if (cruise_mode_status == 1) {
                printf("이미 Cruise Mode 상태입니다.\n");
            }
            else {
                car_brake = 0;
                cancel_btn = 0;
                cruise_mode_status = 1;
                printf("목표속도:%d\n", target_spd);
            }
        }
        else
        {
            break;
        }
        is_fault = alram_flag(current_spd, 1, 1, 0, 0, cruise_btn);
    }
}