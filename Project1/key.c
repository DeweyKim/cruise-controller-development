#include<stdio.h>
#include<conio.h>
#include<windows.h>

int count_test() {
	int counter = 0;
	
	while (1)
	{	
		if (GetAsyncKeyState(VK_UP) != 0)
		{
			printf("Key Down : Set(+)\n");
			//counter 
			if (GetAsyncKeyState(VK_UP) == 0)
			{
				printf("¶¼Áü"); //¿©±â¼­ º»ÇÔ¼ö µ¿ÀÛ -> Bool µ¿ÀÛ
				break; //
			}
		}

		if (GetAsyncKeyState(VK_DOWN) != 0)
		{
			printf("Key Down : Res(-)\n");
			//counter 
			if (GetAsyncKeyState(VK_DOWN) == 0)
			{
				printf("¶¼Áü");
				break;
			}
		}

		if (GetAsyncKeyState(VK_RIGHT) != 0)
		{
			printf("Key Down : Cancel\n");
			//counter 
			if (GetAsyncKeyState(VK_RIGHT) == 0)
			{
				printf("¶¼Áü");
				break;
			}
		}

		if (GetAsyncKeyState(VK_LEFT) != 0)
		{
			printf("Key Down : Cruise\n");
			//counter 
			if (GetAsyncKeyState(VK_LEFT) == 0)
			{
				printf("¶¼Áü");
				break;
			}
		}

	}

	counter = 0; //Ä«¿îÅÍ ÃÊ±âÈ­
	return 0;
	

}


int main() {
	int test = 0;
	int key = 0;
	LARGE_INTEGER BeginTime, EndTime, Frequency;
	double Time;
	
	
	while (1) {
		QueryPerformanceCounter( &BeginTime );
		printf("ÀÔ·Â ´ë±âÁß\n");
		while (1) {
			Time = 0;
			if (_kbhit()) {
				count_test();
				break;
			}
		}
		QueryPerformanceCounter( &EndTime );
		
		QueryPerformanceFrequency( &Frequency );
		Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
		printf("%f\n%f\n%f\n",Time, BeginTime, EndTime);
	}

	return 0;
}
