#include <stdio.h>
#include <conio.h>
#include <windows.h>
#pragma warning(disable:4996)
#define MIN_SPD 50;
#define MAX_SPD 200;
typedef int bool;
#define true 1
#define false 0

typedef enum {
    InputKey_Idle = 0,
    InputKey_Cruise,
    InputKey_SetAccel_Short,
    InputKey_SetAccel_Long,
    InputKey_ResDecel_Short,
    InputKey_ResDecel_Long,
    InputKey_Cancel,
    InputKey_Max
} eInputKey;

int current_spd = 0;
int target_spd = 0;
bool cruise_mode_status;

bool cruise_mode(int current_spd, bool car_brake, bool cancel_button, bool cruise_button, bool is_fault) {
    if (is_fault == 1) {
        target_spd = -1;
        return 0;
    }

    if (cancel_button == 1 && target_spd != -1) {
        target_spd = -1;    
        return 0;
    }

    if (car_brake == 1 && target_spd != -1) {
        return 0;
    }

    if (current_spd < 50) {
        printf("기준속도 미만입니다\n");
        return 0;
    }

    if (current_spd >= 50 && cruise_button == 1) {
        printf("Cruise Mode On\n");
        return 1;
    }

    return 0;
}


int Accelerate(bool InputKey_SetAccel_Short, bool InputKey_SetAccel_Long) {
    if (InputKey_SetAccel_Short == true) {
        target_spd += 1;
    }
    else if (InputKey_SetAccel_Long == true) {
        target_spd += 5;
    }
    else
    {
    	/* exception */
    }
    if (target_spd >= 200) {
        target_spd = 200;
    }
    return target_spd;
}
int Decelerate(bool InputKey_ResDecel_Short, bool InputKey_ResDecel_Long) {
    if (InputKey_ResDecel_Short == true) {
        target_spd -= 1;
    }
    else if (InputKey_ResDecel_Long == true) {
        target_spd -= 5;
    }
    else
    {
    	/* exception */
    }
    if (target_spd < 50) {
        cruise_mode_status = 0;
    }

    return target_spd;
}
int Resume(bool Resume_Button) {
    cruise_mode_status = 1;
    return target_spd;
}
static int readInput(int value)
{
    printf("Input Current Speed : ");
    scanf("%d", &value);
    return value;
}
/*static void pushButton(bool value)
{
    scanf("%d", &value);
    if (current_spd >= 50 && value == 1) {
        Button2 = value;
        printf("===Cruise Mode On===\n");
    }
    else {
        Button2 = false;
        printf("===Cruise Mode Off===\n");
    }
    
}*/
typedef enum {
    CRUISE = 0,
    CANCEL,
    SET,
    RES
} ButtonType;

int Press_Button_Interface() {

    float Time;
    int button_case;
    LARGE_INTEGER BeginTime, EndTime, EndTime300, Frequency;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);

    while (1)
    {
        while ((GetAsyncKeyState(VK_UP)))
        {
            while ((double)(QueryPerformanceCounter(&EndTime300) - BeginTime.QuadPart) / Frequency.QuadPart == 0.3) {
                printf("up\n");
                return InputKey_SetAccel_Long;
            }
            QueryPerformanceCounter(&EndTime);
            button_case = SET;
        }

        while ((GetAsyncKeyState(VK_DOWN)))
        {
            if ((double)(QueryPerformanceCounter(&EndTime300) - BeginTime.QuadPart) / Frequency.QuadPart == 0.3) {
                return InputKey_ResDecel_Short;
            }
            QueryPerformanceCounter(&EndTime);
            button_case = RES;
        }

        while ((GetAsyncKeyState(VK_RIGHT)))
        {
            QueryPerformanceCounter(&EndTime);
            button_case = CANCEL;
        }

        while ((GetAsyncKeyState(VK_LEFT)))
        {
            QueryPerformanceCounter(&EndTime);
            button_case = CRUISE;
        }

        _getch();
        break;
    }

    Time = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time = Time * 1000;
    //printf("%lf\n", Time);

    if (Time >= 50 && Time < 300) {
        if (button_case == SET) {
            return InputKey_SetAccel_Short;
        }
        else if (button_case == CANCEL) {
            return InputKey_Cancel;
        }
        else if (button_case == CRUISE) {
            return InputKey_Cruise;
        }
        else if (button_case == RES) {
            return InputKey_ResDecel_Short;
        }
        else return InputKey_Idle;
    }
    else if (Time < 50) {
        return InputKey_Idle;
    }
    else if (Time >= 300) {
        if (button_case == SET) {
            return InputKey_SetAccel_Long;
        }
        else if (button_case == RES) {
            return InputKey_ResDecel_Long;
        }
        else return InputKey_Idle;
    }
}
char alram_flag(veh_speed, veh_ready, lgon, can_fault, can_timeout, cruise_button)
{
    char alarmflag;
    /*int veh_speed = 0;
    bool veh_ready = true;
    bool lgon = true;
    bool can_fault = false;
    bool can_timeout = false;
    bool cruise_button = false;
    */
    if (veh_ready == false) {//차량 Ready 해제
        alarmflag = "0x00";
        return alarmflag;
    }
    if (lgon == false) {//lgOn신호 Reset
        alarmflag = "0x01";
        return alarmflag;
    }
    if (can_fault == true) {//Can Fault
        alarmflag = "0x02";
        return alarmflag;
    }
    if (can_timeout == true) {//Can Timeout
        alarmflag = "0x03";
        return alarmflag;
    }
    if (veh_speed < 50) { // 현재 차속 50km 미만인 경우 버튼 입력 받았을 때
        if (cruise_button == true) {
            alarmflag = "0x04";
            return alarmflag;
        }
    }
    return 0;
}
int main(int argc, char* argv[]) {
    
    bool car_brake = 0;       //브레이크 동작상태
    bool cancel_btn = 0;        //Cancel버튼 동작상태
    bool cruise_btn = 1;        //크루즈버튼 동작상태
    bool is_fault = 0;          //Fault처리
    
    eInputKey e;
    ButtonType b;

    int test = 0;
    int key = 0;

    int s = 0;

    int STATUS_KEY=0; // 입력버튼 상태 값

    int value=0;
    current_spd = readInput(value);

    while (STATUS_KEY!= InputKey_Cancel) {
        if (_kbhit()) {
            STATUS_KEY = Press_Button_Interface();
            if (STATUS_KEY == InputKey_Cruise){
                cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, 1, is_fault);
                if (cruise_mode_status == 0) {
                    current_spd = readInput(value);
                }
            }
            _getch();
        }
        is_fault = alram_flag(current_spd, 1, 1, 0, 0, cruise_btn);
    }
    printf("Cruise Mode Off\n");
    printf("%d", current_spd);
    
	return 0;
}   
