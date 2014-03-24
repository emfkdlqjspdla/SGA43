#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <cmath>
#include <list>
#include <fstream>

using namespace std;

struct ItemInfo
{
	char* name[20];
	int price;
};

int main(void)
{
	//ItemInfo item;

	//for (int i = 0; i < 10; i++)
	//{
	//	cout << "name : ";
	//	cin >> item.name;
	//	cout << "price : ";
	//	cin >> item.price;

	//	// write to file
	//}

	// read from file.


	fstream file;

	file.open("data.dat", ios_base::out | ios_base::binary);

	int a = 11;

	file.write((char*)&a, sizeof(int));

	file.close();

	_getch();
	return 0;
}
