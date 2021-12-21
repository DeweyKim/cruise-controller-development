#include <stdio.h>
#include <conio.h>
#include <windows.h>

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
    printf("%lf\n", Time);

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