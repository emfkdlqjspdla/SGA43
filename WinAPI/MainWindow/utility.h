#pragma once

template<typename T>
void SafeDelete(T& pointer)
{
	if (pointer)
	{
		delete [] pointer;
		pointer = NULL;
	}
}

template<typename Char>
void strAlloc(Char** dst, const Char* src)
{
	int len = 0;
	while (src[len++]);

	*dst = new Char[len];
	len = 0;
	while ((*dst)[len++] = src[len]);
}
