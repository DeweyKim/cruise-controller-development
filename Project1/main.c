#include <stdio.h>
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
eInputKey Button1 = InputKey_Idle;
eInputKey Button2 = InputKey_Cruise;
eInputKey Button3 = InputKey_SetAccel_Short;
eInputKey Button4 = InputKey_SetAccel_Long;
eInputKey Button5 = InputKey_ResDecel_Short;
eInputKey Button6 = InputKey_ResDecel_Long;
eInputKey Button7 = InputKey_Cancel;
eInputKey Button8 = InputKey_Max;
int current_spd = 0;
int target_spd = 0;
bool cruise_mode_status;
bool cruise_mode(bool car_break, bool cancel_button, bool cruise_button, bool is_fault) {
    if (is_fault == 1) {
        target_spd = -1;
        return 0;
    }

    if (cancel_button == 1 && target_spd != -1) {
        target_spd = -1;    
        return 0;
    }

    if (car_break == 1 && target_spd != -1) {
        return 0;
    }

    if (current_spd < 50) {
        return 0;
    }

    if (current_spd >= 50 && cruise_button == 1) {
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
    return target_spd;
}
static void readInput(int value)
{
    printf("Input Current Speed : ");
    scanf("%d", &value);
    current_spd = value;
    target_spd = value;
}
static void pushButton(bool value)
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
    
}
int main(int argc, char* argv[]) {
    
    bool car_brake = 0;         //브레이크 동작상태

    bool cancel_btn = 0;        //Cancel버튼 동작상태
    bool cruise_btn = 1;        //크루즈버튼 동작상태

    bool is_fault = 0;          //Fault처리

    readInput(&current_spd);
    cruise_mode_status=cruise_mode(current_spd, target_spd, car_brake, cancel_btn, cruise_btn, is_fault);
    if (cruise_mode_status == 1) {
        printf("Cruise Mode On");
    }
    else {
        printf("Cruise Mode Off");
    }
    

   
	return 0;
}   




