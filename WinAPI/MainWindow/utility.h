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

template<typename Cls>
class singleton
{
protected :
	singleton(){}
	virtual ~singleton(){}

public :
	static Cls& getReference(void)
	{
		static Cls inst;
		return inst;
	}
};

template<typename ReturnType, typename Class, typename FunctionPointer>
class Callable
{
public :
	Callable(FunctionPointer a):func(a){}

	// argument 4
	template<typename Derived,
			typename Arg1, 
			typename Arg2, 
			typename Arg3, 
			typename Arg4>
	ReturnType operator ()(Derived* pObj, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		return (get_pointer(pObj)->*func)(a1, a2, a3, a4);
	}
	// argument 3
	template<typename Derived,
			typename Arg1, 
			typename Arg2, 
			typename Arg3>
	ReturnType operator ()(Derived* pObj, Arg1 a1, Arg2 a2, Arg3 a3)
	{
		return (get_pointer(pObj)->*func)(a1, a2, a3);
	}
	// argument 2
	template<typename Derived,
			typename Arg1, 
			typename Arg2>
	ReturnType operator ()(Derived* pObj, Arg1 a1, Arg2 a2)
	{
		return (get_pointer(pObj)->*func)(a1, a2);
	}
	// argument 1
	template<typename Derived,
			typename Arg1>
	ReturnType operator ()(Derived* pObj, Arg1 a1)
	{
		return (get_pointer(pObj)->*func)(a1);
	}
	// argument 0
	template<typename Derived>
	ReturnType operator ()(Derived* pObj)
	{
		return (get_pointer(pObj)->*func)();
	}

	template<typename Derived>
	Class* get_pointer(Derived* obj)
	{
		return dynamic_cast<Class*>(obj);
	}
private :
	FunctionPointer func;
};

template<typename T>
T Select(HDC hdc, T GDI_Object)
{
	return T(::SelectObject(hdc, GDI_Object));
}