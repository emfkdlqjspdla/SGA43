#include <iostream>
#include <conio.h>
#include <tchar.h>

using namespace std;

int foo(void)
{
	cout << "hello" << endl;

	return 0;
}

int main(void)
{
	typedef int(*FunctionPointer)(void);

	FunctionPointer a = &foo;

	(*a)();
	//


	_getch();
	return 0;
}