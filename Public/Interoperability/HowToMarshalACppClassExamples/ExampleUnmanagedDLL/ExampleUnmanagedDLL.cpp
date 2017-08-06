#include <iostream>

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "ExampleUnmanagedDLL.h"



#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain(
	HMODULE		/*hModule*/,
	DWORD		ul_reason_for_call,
	LPVOID		/*lpReserved*/)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

CUnmanagedTestClass::CUnmanagedTestClass()
{
}

CUnmanagedTestClass::~CUnmanagedTestClass()
{
}

void CUnmanagedTestClass::PassInt(int nValue)
{
	std::cout << "PassInt() was called" << std::endl;
	std::cout << "The value is " << nValue << std::endl;
}

void CUnmanagedTestClass::PassString(char* pchValue)
{
	std::cout << "PassString() was called" << std::endl;
	std::cout << "The string is " << pchValue << std::endl;
}

char* CUnmanagedTestClass::ReturnString()
{
	static char s_chString[] = "Hello there";
	std::cout << "ReturnString() was called" << std::endl;

	return s_chString;
}
