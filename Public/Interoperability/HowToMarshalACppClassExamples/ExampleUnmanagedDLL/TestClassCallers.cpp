#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "TestClassCallers.h"


extern "C" EXAMPLEUNMANAGEDDLL_API CUnmanagedTestClass* CreateTestClass()
{
	return new CUnmanagedTestClass();
}

extern "C" EXAMPLEUNMANAGEDDLL_API void DisposeTestClass(CUnmanagedTestClass* pObject)
{
	if(pObject != NULL)
	{
		delete pObject;
		pObject = NULL;
	}
}

extern "C" EXAMPLEUNMANAGEDDLL_API void CallPassInt(CUnmanagedTestClass* pObject, int nValue)
{
	if(pObject != NULL)
	{
		pObject->PassInt(nValue);
	}
}

extern "C" EXAMPLEUNMANAGEDDLL_API void CallPassString(CUnmanagedTestClass* pObject, char* pchValue)
{
	if(pObject != NULL)
	{
		pObject->PassString(pchValue);
	}
}

extern "C" EXAMPLEUNMANAGEDDLL_API char* CallReturnString(CUnmanagedTestClass* pObject)
{
	if(pObject != NULL)
	{
		return pObject->ReturnString();
	}

	return NULL;
}
