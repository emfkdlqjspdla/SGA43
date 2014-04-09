#pragma once

#include "..\MainWindow\image.h"

class Button
{
public :
	void SetImageOn(LPCTSTR szFileName, const Rect& rc)
	{
		imgOn.load(szFileName, rc);
	}
	void SetImageOff(LPCTSTR szFileName, const Rect& rc)
	{
		imgOff.load(szFileName, rc);
	}

private :
	Image imgOn;
	Image imgOff;
};