#pragma once

#include "..\MainWindow\sga.hpp"
#include <bitset>

class InputProcessor : public singleton<InputProcessor>
{
	friend class singleton<InputProcessor>;

	typedef InputProcessor Me;
	enum {count = 256};
private :
	InputProcessor()
	{
		::memset(ckey, 0, sizeof(BYTE)*count);
	}
	~InputProcessor(){}
public :
	void Update(DWORD tick)
	{
		::memcpy(pkey, ckey, sizeof(BYTE)*count);
		::GetKeyboardState(ckey);

		_check();
	}

	bool operator [] (const int& vkey)
	{
		return ((ckey[vkey] & 0x80) == 0x80);
	}

	bool OnClick(const int& vkey)
	{
		return _OnClick[vkey];
	}

private :
	void _check()
	{
		for (int i = 0; i < count; i++)
		{
			if (((ckey[i] & 0x80) == 0x00) && ((pkey[i] & 0x80) == 0x80))
			{
				_OnClick[i] = true;
			}
			else
			{
				_OnClick[i] = false;
			}
		}
	}

private :
	BYTE ckey[count];
	BYTE pkey[count];

	std::bitset<count> _OnClick;
};

#define InputDevice InputProcessor::getReference()