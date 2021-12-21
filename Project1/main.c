#include <stdio.h>
#include <stdbool.h>

//cruise_mode_status에서 cruise mode on은 1, cruise mode off는 0
int cruise_mode(int current_car_speed, int *target_car_speed, bool car_break, bool cancel_button, bool *cruise_mode_status, bool cruise_button){

	if (cancel_button == 1 && *target_car_speed != -1) {
		*cruise_mode_status = 0;
		*target_car_speed = -1;
		return 0;
	}

	if (car_break == 1 && *target_car_speed != -1) {
		*cruise_mode_status = 0;
		return 0;
	}

	if (cruise_button == 1 && current_car_speed >= 50) {
		*cruise_mode_status = 1;
	}

	if (*cruise_mode_status == 1) {
		current_car_speed = *target_car_speed;
	}
	return 0;
}

int main() {
	printf("Hello World!");

	return 0;
}