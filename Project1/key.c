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
				printf("����"); //���⼭ ���Լ� ���� -> Bool ����
				break; //
			}
		}

		if (GetAsyncKeyState(VK_DOWN) != 0)
		{
			printf("Key Down : Res(-)\n");
			//counter 
			if (GetAsyncKeyState(VK_DOWN) == 0)
			{
				printf("����");
				break;
			}
		}

		if (GetAsyncKeyState(VK_RIGHT) != 0)
		{
			printf("Key Down : Cancel\n");
			//counter 
			if (GetAsyncKeyState(VK_RIGHT) == 0)
			{
				printf("����");
				break;
			}
		}

		if (GetAsyncKeyState(VK_LEFT) != 0)
		{
			printf("Key Down : Cruise\n");
			//counter 
			if (GetAsyncKeyState(VK_LEFT) == 0)
			{
				printf("����");
				break;
			}
		}

	}

	counter = 0; //ī���� �ʱ�ȭ
	return 0;
	

}


int main() {
	int test = 0;
	int key = 0;
	LARGE_INTEGER BeginTime, EndTime, Frequency;
	double Time;
	
	
	while (1) {
		QueryPerformanceCounter( &BeginTime );
		printf("�Է� �����\n");
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
