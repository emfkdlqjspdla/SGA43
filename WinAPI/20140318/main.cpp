#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <cmath>
#include <list>

using namespace std;

int main(void)
{
	list<int> test;

	for (int i = 0; i < 10; i++)
		test.push_back(i);

	list<int>::iterator it;
	for (it = test.begin(); it != test.end(); it++)
	{
		cout << *it << endl;
	}


	for (int i = 0; i < 10; i++)
		test.pop_front();

	_getch();
	return 0;
}
