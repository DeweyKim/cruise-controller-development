#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#pragma warning(disable:4996)

#define MIN_SPD 50
#define MAX_SPD 200

#define true 1
#define false 0

typedef int int8_t;
typedef float float8_t;

typedef enum {
    InputKey_Idle = 0,
    InputKey_Cruise,
    InputKey_SetAccel_Short,
    InputKey_SetAccel_Long,
    InputKey_ResDecel_Short,
    InputKey_ResDecel_Long,
    InputKey_Cancel,
    InputKey_Max,
    InputKey_Brake,
    InputKey_Resume,
    InputKey_Exit
} eInputKey;

typedef enum {
    CRUISE = 0,
    CANCEL,
    SET,
    RES
} ButtonType;

int8_t cruise_mode(int8_t current_spd, int8_t car_brake, int8_t cancel_button, int8_t cruise_button, int8_t is_fault);
int8_t Accelerate(int8_t InputKey_SetAccel_Short, int8_t InputKey_SetAccel_Long);
int8_t Decelerate(int8_t InputKey_ResDecel_Short, int8_t InputKey_ResDecel_Long);
static int8_t readInput(int8_t value);
static int8_t Press_Button_Interface(void);
static int8_t alram_flag(int8_t veh_speed, int8_t veh_ready, int8_t lgon, int8_t can_fault, int8_t can_timeout, int8_t cruise_button);
int8_t main(int8_t argc, char* argv[]);

int8_t current_spd;
static int8_t target_spd;
static int8_t extra_time;
static int8_t cruise_mode_status;

current_spd = 0;
target_spd = 0;
extra_time = 0;

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
static int8_t readInput(int8_t value)
{
    printf("Input Current Speed : ");
    scanf("%d", &value);
    return value;
}
static int8_t Press_Button_Interface(void) {
    float8_t Time, Time1;
    int8_t button_case;
    LARGE_INTEGER BeginTime, EndTime, Frequency;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);

    while (1)
    {
        while (GetAsyncKeyState(VK_UP))
        {
            QueryPerformanceCounter(&EndTime);
            if (Frequency.QuadPart != 0) {
                Time1 = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
                Time1 *= 1000.0;
                if (floor(Time1) == 300.0) {
                    //printf("====== %d\n", extra_time);
                    extra_time = 1;
                    return InputKey_SetAccel_Long;
                }
            }
            else {
                /*exception*/
            }
            button_case = SET;
        }

        while (GetAsyncKeyState(VK_DOWN))
        {
            QueryPerformanceCounter(&EndTime);
            if (Frequency.QuadPart != 0) {
                Time1 = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
                Time1 *= 1000.0;
                if (floor(Time1) == 300.0) {
                    //printf("====== %d\n", extra_time);
                    extra_time = 1;
                    return InputKey_ResDecel_Long;
                }
                button_case = RES;
            }
            else {
                /*exception*/
            }
        }

        while (GetAsyncKeyState(VK_RIGHT))
        {
            button_case = CANCEL;
        }

        while (GetAsyncKeyState(VK_LEFT))
        {
            button_case = CRUISE;
        }

        while (GetAsyncKeyState(0x51))
        {
            return InputKey_Exit;
        }

        while (GetAsyncKeyState(0x42))
        {
            return InputKey_Brake;
        }

        while (GetAsyncKeyState(0x52))
        {
            return InputKey_Resume;
        }

        QueryPerformanceCounter(&EndTime);
        _getch();
        break;
    }

    if (Frequency.QuadPart != 0) {
        Time = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
        Time = Time * 1000.0;
        //printf("%lf\n", Time);
    }
    else {
        /*exception*/
    }

    if ((Time >= 50.0) && (Time < 300.0)) {
        if (extra_time == 0) {
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
            else { return InputKey_Idle; }
        }
        else { //300이상 눌린 상태에서 나머지 extra time 처리
            extra_time = 0;
            return InputKey_Idle;
        }
    }
    else if (Time < 50) {
        extra_time = 0;
        return InputKey_Idle;
    }
    else if (Time >= 300) {
        if (button_case == SET) {
            return InputKey_SetAccel_Long;
        }
        else if (button_case == RES) {
            return InputKey_ResDecel_Long;
        }
        else { return InputKey_Idle; }
    }
    else
    {
        /* exception */
    }

    return 0;
}
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
    if (veh_ready == false) {       //차량 Ready 해제
        alarmflag = 0x00;
        return alarmflag;
    }
    if (lgon == false) {            //lgOn신호 Reset
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
    if (veh_speed < 50) {           // 현재 차속 50km 미만인 경우 버튼 입력 받았을 때
        if (cruise_button == true) {
            alarmflag = 0x04;
            return alarmflag;
        }
    }
    return 0;
}
int8_t main(int8_t argc, char* argv[]) {

    int8_t car_brake = 0;         //브레이크 동작상태
    int8_t cancel_btn = 0;        //Cancel버튼 동작상태
    int8_t cruise_btn = 0;        //크루즈버튼 동작상태
    int8_t is_fault = 0;          //Fault처리

    int8_t STATUS_KEY = 0; // 입력버튼 상태 값
    int8_t value = 0;

    current_spd = readInput(value);
    target_spd = current_spd;

    while (STATUS_KEY != InputKey_Exit) {
        STATUS_KEY = Press_Button_Interface();
        while ((STATUS_KEY != InputKey_Cancel) && (STATUS_KEY != InputKey_Brake)) {
            //STATUS_KEY = Press_Button_Interface();
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
                printf("목표속도:%d\n", Accelerate(1,0));
            }
            else if ((STATUS_KEY == InputKey_SetAccel_Long) && (cruise_mode_status == 1)) {
                printf("목표속도:%d\n", Accelerate(0, 1));
            }
            else if ((STATUS_KEY == InputKey_ResDecel_Short) && (cruise_mode_status == 1)) {
                printf("목표속도:%d\n", Decelerate(1, 0));
            }
            else if ((STATUS_KEY == InputKey_ResDecel_Long) && (cruise_mode_status == 1)) {
                printf("목표속도:%d\n", Decelerate(0, 1));
            }
            else if (((STATUS_KEY == InputKey_SetAccel_Long) || (STATUS_KEY == InputKey_SetAccel_Short) || (STATUS_KEY == InputKey_ResDecel_Long) || (STATUS_KEY == InputKey_ResDecel_Short)) && (cruise_mode_status == 0)) {
                printf("Cruise Mode Off 상태입니다.\n");
            }
            else if (STATUS_KEY == InputKey_Resume && cruise_mode_status == 0) {
                cruise_btn = 1;
                cancel_btn = 0;
                car_brake = 0;
                cruise_mode_status = 1;
                printf("목표속도:%d\n", target_spd);
                _getch();
            }
            else if (STATUS_KEY == InputKey_Exit) {
                break;
            }
            else
            {
                /* exception */
            }

            STATUS_KEY = Press_Button_Interface();
            //_getch();
        }
        while (STATUS_KEY == InputKey_Cancel) {
            car_brake = 0;
            cancel_btn = 1;
            cruise_btn = 0;
            cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, cruise_btn, is_fault);
            printf("000\n");
            //_getch();
            break;
        }
        while (STATUS_KEY == InputKey_Brake) {
            car_brake = 1;
            cancel_btn = 0;
            cruise_btn = 0;
            cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, cruise_btn, is_fault);
            printf("001\n");
            //_getch();
            break;
        }
        /*while (STATUS_KEY == InputKey_Resume) {
            STATUS_KEY = Press_Button_Interface();
            break;
        }*/
        is_fault = alram_flag(current_spd, 1, 1, 0, 0, cruise_btn);
        printf("Cruise Mode Off\n");
        _getch();
    }
    return 0;
}