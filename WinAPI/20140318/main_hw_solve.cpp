#include <iostream>
#include <conio.h> // console input output, _getch()

using namespace std;

void func1(void)
{
	cout << "fun1 called." << endl;
}
void func2(void)
{
	cout << "fun2 called." << endl;
}
void func3(void)
{
	cout << "fun3 called." << endl;
}
void func4(void)
{
	cout << "fun4 called." << endl;
}

int main(void)
{
	typedef void(*FunctionPointer)(void);

	FunctionPointer fp[4] = {&func1, &func2, &func3, &func4};

	for (int i = 0; i < 4; i++)
	{
		cout << i+1 << ". Func" << i+1 << endl;
	}

	int input;

	do {
		cout << "Input : ";
		cin >> input;
		if (!(input >= 1 && input <= 4))
			cout << "retry " << endl;
	} while (!(input >= 1 && input <= 4));

	(*fp[input-1])();

	_getch();
	return 0;
}