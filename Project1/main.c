#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//cruise_mode_status에서 cruise mode on은 1, cruise mode off는 0
int cruise_mode(int *current_car_speed, int *target_car_speed, bool car_break, bool cancel_button, bool *cruise_mode_status, bool cruise_button, bool is_fault) {
	if (is_fault == 1){
		*cruise_mode_status = 0;
		*target_car_speed = -1;
		return 0;
	}

	if (cancel_button == 1 && *target_car_speed != -1) {
		*cruise_mode_status = 0;
		*target_car_speed = -1;
		return 0;
	}

	if (car_break == 1 && *target_car_speed != -1) {
		*cruise_mode_status = 0;
		return 0;
	}

	if (*current_car_speed < 50) {
		*cruise_mode_status = 0;
		return 0;
	}

	if (cruise_button == 1 && *current_car_speed >= 50) {
		*cruise_mode_status = 1;
	}

	if (*cruise_mode_status == 1) {
		if (abs(*current_car_speed - *target_car_speed) <= 1) {
			*current_car_speed = *target_car_speed;
		}
		else if (*current_car_speed < *target_car_speed) {
			(*current_car_speed) += 2;
		}
	}
	return 0;
}

int main() {
	int current_car_speed = 0;
	int target_car_speed = 0;
	bool cruise_status = 0;
	bool car_break = 0;
	bool cancel_button = 0;
	bool cruise_button = 0;
	bool is_fault = 0;

	while (1) {
		//브레이크, 캔슬, 크루즈버튼은 키 입력 받아야 함 / fault는 값 입력 받아야 함
		cruise_mode(&current_car_speed, &target_car_speed, car_break, cancel_button, &cruise_status, cruise_button, is_fault);
	}

	return 0;
}