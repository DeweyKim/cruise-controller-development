#include <stdio.h>

//cruise_mode_status에서 cruise mode on은 1, cruise mode off는 0
int cruise_mode(float current_car_speed, float *target_car_speed, char car_break, char cancel_button, char *cruise_mode_status, char cruise_button){
	float saved_target_car_speed = 0;

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