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
	// �Լ������� Ÿ�� ������
	typedef int(*FunctionPointer)(void);

	// �Լ������� ������ foo �� �ּҸ� ����.
	FunctionPointer a = &foo;

	(*a)();
	

	_getch();
	return 0;
}
