#include <iostream>
#include <conio.h>
#include <map>

using namespace std;

int main(void)
{
	map</*key*/const char*, /*data*/const char*> test;

	test.insert(make_pair("hello", "helloa"));
	test.insert(make_pair("world", "worlda"));
	test.insert(make_pair("mi", "good job"));

	map<const char*,const char*>::iterator it;
	it = test.find("hello");
	it->second = "bye";

	cout << test["hello"] << endl;

	test["hello"] = "byebye";

	cout << test["hello"] << endl;


	_getch();
	return 0;
}
