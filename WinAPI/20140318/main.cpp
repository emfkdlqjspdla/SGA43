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
	// 함수포인터 타입 재정의
	typedef int(*FunctionPointer)(void);

	// 함수포인터 변수에 foo 의 주소를 저장.
	FunctionPointer a = &foo;

	(*a)();
	

	_getch();
	return 0;
}
