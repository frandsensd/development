#ifndef __TestClassCallers_h__
#define __TestClassCallers_h__

#include "ExampleUnmanagedDLL.h"		// needed for EXAMPLEUNMANAGEDDLL_API

#ifdef __cplusplus
extern "C" {
#endif

extern EXAMPLEUNMANAGEDDLL_API CUnmanagedTestClass* CreateTestClass();
extern EXAMPLEUNMANAGEDDLL_API void DisposeTestClass(CUnmanagedTestClass* pObject);

extern EXAMPLEUNMANAGEDDLL_API void CallPassInt(CUnmanagedTestClass* pObject, int nValue);
extern EXAMPLEUNMANAGEDDLL_API void CallPassString(CUnmanagedTestClass* pObject, char* pchValue);
extern EXAMPLEUNMANAGEDDLL_API char* CallReturnString(CUnmanagedTestClass* pObject);

#ifdef __cplusplus
}
#endif

#endif // __TestClassCallers_h__

