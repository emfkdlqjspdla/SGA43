#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <cmath>

using namespace std;

int main(void)
{
/*
sin(theta_rad) = value
theta_rad = asin(value)

sin(pi/2) = 1

pi/2 = asin(1)
pi = 2*asin(1)

*/
	float pi = 2.f*asin(1.f);
	float D2R = pi/180.f;

	for (int i = 0; i <= 360; i++)
	{
		cout << "cos(" << i << ") = " << cos(i*1.0f*D2R) << endl;
	}

	_getch();
	return 0;
}
