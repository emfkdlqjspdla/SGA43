#include <iostream>
#include <conio.h>

using namespace std;

class Test
{
public :
	void Func1(void)
	{
		cout << "Test::Func1 called." << endl;
	}
	void Func2(void)
	{
		cout << "Test::Func2 called." << endl;
	}
};


int main(void)
{
	Test a;

	typedef void (Test::*MemberFunctionPointer)(void);

	MemberFunctionPointer mfp[2] = {&Test::Func1, &Test::Func2};

	(a.*mfp[0])();


	_getch();
	return 0;
}