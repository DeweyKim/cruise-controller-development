#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#pragma warning(disable:4996)

#define LONG_TIME 300
#define MIN_SPD 50
#define MAX_SPD 200

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

bool cruise_mode(int current_spd, bool car_brake, bool cancel_button, bool cruise_button, bool is_fault);
int Accelerate(bool InputKey_SetAccel_Short, bool InputKey_SetAccel_Long);
int Decelerate(bool InputKey_ResDecel_Short, bool InputKey_ResDecel_Long);
static int readInput(int value);
int Press_Button_Interface();
int alram_flag(bool veh_speed, bool veh_ready, bool lgon, bool can_fault, bool can_timeout, bool cruise_button);
int main(int argc, char* argv[]);

int current_spd = 0;
int target_spd = 0;
bool cruise_mode_status;
int extra_time = 0;

bool cruise_mode(int current_spd, bool car_brake, bool cancel_button, bool cruise_button, bool is_fault) {
    if (is_fault == 1) {
        target_spd = -1;
        return 0;
    }

    else if ((cancel_button == 1) && (target_spd != -1)) {
        target_spd = -1;
        return 0;
    }

    else if ((car_brake == 1) && (target_spd != -1)) {
        return 0;
    }

    else if (current_spd < 50) {
        printf("���ؼӵ� �̸��Դϴ�\n");
        return 0;
    }

    else if ((current_spd >= 50) && (cruise_button == 1)) {
        printf("Cruise Mode On\n");
        target_spd = current_spd;
        return 1;
    }
    else
    {
    	/* exception */
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
    if (target_spd >= MAX_SPD) {
        target_spd = MAX_SPD;
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
    if (target_spd < MIN_SPD) {
        target_spd = MIN_SPD;
    }

    return target_spd;
}
static int readInput(int value)
{
    printf("Input Current Speed : ");
    scanf("%d", &value);
    return value;
}
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
                //printf("====== %d\n", extra_time);
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
                //printf("====== %d\n", extra_time);
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

        while ((GetAsyncKeyState(0x51)))
        {
            return InputKey_Exit;
        }

        while ((GetAsyncKeyState(0x42)))
        {
            return InputKey_Brake;
        }

        while ((GetAsyncKeyState(0x52)))
        {
            return InputKey_Resume;
        }

        QueryPerformanceCounter(&EndTime);
        _getch();
        break;
    }

    Time = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time = Time * 1000;
    //printf("%lf\n", Time);

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
            else  { return InputKey_Idle; } 
        }
        else { //300�̻� ���� ���¿��� ������ extra time ó��
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
        else  { return InputKey_Idle; } 
    }
    else
    {
    	/* exception */
    }

    return 0;
}
int alram_flag(bool veh_speed, bool veh_ready, bool lgon, bool can_fault, bool can_timeout, bool cruise_button)
{
    int alarmflag;
    /*int veh_speed = 0;
    bool veh_ready = true;
    bool lgon = true;
    bool can_fault = false;
    bool can_timeout = false;
    bool cruise_button = false;
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
int main(int argc, char* argv[]) {

    bool car_brake = 0;         //�극��ũ ���ۻ���
    bool cancel_btn = 0;        //Cancel��ư ���ۻ���
    bool cruise_btn = 1;        //ũ�����ư ���ۻ���
    bool is_fault = 0;          //Faultó��

    eInputKey e;
    ButtonType b;

    int STATUS_KEY = 0; // �Է¹�ư ���� ��

    int value = 0;

    current_spd = readInput(value);
    target_spd = current_spd;

    while (STATUS_KEY != InputKey_Exit) {
        if (_kbhit()) {
            STATUS_KEY = Press_Button_Interface();
            while (STATUS_KEY != InputKey_Cancel && STATUS_KEY != InputKey_Brake) {
                STATUS_KEY = Press_Button_Interface();
                if (STATUS_KEY == InputKey_Cruise) {
                    cruise_mode_status = cruise_mode(current_spd, car_brake, cancel_btn, 1, is_fault);
                    if (cruise_mode_status == 0) {
                        current_spd = readInput(value);
                        target_spd = current_spd;
                    }
                }
                else if (STATUS_KEY == InputKey_SetAccel_Short && cruise_mode_status == 1) {
                    Accelerate(1, 0);
                    printf("��ǥ�ӵ�:%d\n", target_spd);
                }
                else if (STATUS_KEY == InputKey_SetAccel_Long && cruise_mode_status == 1) {
                    Accelerate(0, 1);
                    printf("��ǥ�ӵ�:%d\n", target_spd);
                }
                else if (STATUS_KEY == InputKey_ResDecel_Short && cruise_mode_status == 1) {
                    Decelerate(1, 0);
                    printf("��ǥ�ӵ�:%d\n", target_spd);
                }
                else if (STATUS_KEY == InputKey_ResDecel_Long && cruise_mode_status == 1) {
                    Decelerate(0, 1);
                    printf("��ǥ�ӵ�:%d\n", target_spd);
                }
                else if ((STATUS_KEY == InputKey_SetAccel_Long || STATUS_KEY == InputKey_SetAccel_Short || STATUS_KEY == InputKey_ResDecel_Long || STATUS_KEY == InputKey_ResDecel_Short) && cruise_mode_status == 0) {
                    printf("Cruise Mode OFF �����Դϴ�.\n");
                }
                else if (STATUS_KEY == InputKey_Resume) {
                    cruise_mode_status = 1;
                    printf("��ǥ�ӵ�:%d\n", target_spd);
                }
                else if (STATUS_KEY == InputKey_Exit) {
                    break;
                }
                else
                {
                	/* exception */
                }
                _getch();
            }
            while (STATUS_KEY == InputKey_Cancel) {
                target_spd = -1;
                cruise_mode_status = 0;
                printf("000\n");
                //_getch();
                break;
            }
            while (STATUS_KEY == InputKey_Brake) {
                cruise_mode_status = 0;
                printf("001\n");
                //_getch();
                break;
            }
            is_fault = alram_flag(current_spd, 1, 1, 0, 0, cruise_btn);
            printf("Cruise Mode Off\n");
            _getch();
        }
    }

    target_spd = -1;
    printf("Cruise Mode Off\n");
    printf("%d", current_spd);
    
	return 0;
}   