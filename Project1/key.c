#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <math.h>

#define LONG_TIME 300
int extra_time = 0;

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

typedef enum {
    CRUISE = 0,
    CANCEL,
    SET,
    RES
} ButtonType;

int Press_Button_Interface() {

    float Time, Time1;
    int button_case;
    LARGE_INTEGER BeginTime, EndTime, Frequency;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);


    while (1)
    {

        while ((GetAsyncKeyState(VK_UP)))
        {
            QueryPerformanceCounter(&EndTime);
            Time1 = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
            Time1 *= 1000;
            if (floor(Time1) == 300) {
                printf("====== %d\n", extra_time);
                extra_time = 1;
                return InputKey_SetAccel_Long;
            }
            button_case = SET;
        }

        while ((GetAsyncKeyState(VK_DOWN)))
        {
            QueryPerformanceCounter(&EndTime);
            Time1 = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
            Time1 *= 1000;
            if (floor(Time1) == 300) {
                printf("====== %d\n", extra_time);
                extra_time = 1;
                return InputKey_ResDecel_Long;
            }
            button_case = RES;
        }

        while ((GetAsyncKeyState(VK_RIGHT)))
        {
            button_case = CANCEL;
        }

        while ((GetAsyncKeyState(VK_LEFT)))
        {
            button_case = CRUISE;
        }

        QueryPerformanceCounter(&EndTime);
        _getch();
        break;
    }

    Time = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time = Time * 1000;
    printf("%lf\n", Time);

    if (Time >= 50 && Time < 300) {

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
            else return InputKey_Idle;
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
        else return InputKey_Idle;
    }
}

int main() {
    eInputKey e;
    ButtonType b;

    int test = 0;
    int key = 0;

    int s = 0;

    int STATUS_KEY; // 입력버튼 상태 값

    while (1) {
        printf("입력 대기중\n");

        while (1) {
            if (_kbhit()) {
                STATUS_KEY = Press_Button_Interface();
                printf("%d\n", STATUS_KEY);
                _getch();
                break;
            }
        }
    }

    return 0;
}