#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <cmath>
#include <list>
#include <fstream>

using namespace std;

struct ItemInfo
{
	char name[20];
	int price;
};

int main(void)
{
	char* menu[] = {
		"0. exit",
		"1. load",
		"2. save"
	};

	ItemInfo item;

	bool bExit = false;
	while (!bExit)
	{
		// menu print
		for (int i = 0; i < 3; i++)
		{
			cout << menu[i] << endl;
		}
		int input;
		cout << "select : ";
		cin >> input;

		switch (input)
		{
		case 0 :
			{
				bExit = true;
			}
			break;
		case 2 :
			{
				fstream outfile;
				outfile.open("itemlist.dat", ios_base::out | ios_base::binary);
				for (int i = 0; i < 2; i++)
				{
					cin >> item.name;
					cin >> item.price;

					outfile.write((const char*)&item, sizeof(ItemInfo));

				}

				outfile.close();
			}
			break;
		case 1 :
			{
				fstream infile;
				infile.open("itemlist.dat", ios_base::in | ios_base::binary);
				for (int i = 0; i < 2; i++)
				{
					infile.read((char*)&item, sizeof(ItemInfo));

					cout << item.name << " ";
					cout << item.price << endl;
				}

				infile.close();
			}
			break;
		default :
			{
			}
			break;
		}


	}

	fstream file;

	file.open("data.dat", ios_base::out | ios_base::binary);

	int a = 11;

	file.write((char*)&a, sizeof(int));

	file.close();

	_getch();
	return 0;
}
