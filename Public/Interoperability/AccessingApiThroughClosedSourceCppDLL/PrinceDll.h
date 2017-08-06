// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PRINCEDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PRINCEDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PRINCEDLL_EXPORTS
#define PRINCEDLL_API __declspec(dllexport)
#else
#define PRINCEDLL_API __declspec(dllimport)
#endif


#include <tchar.h>
#include <strsafe.h>
#include <Windows.h>
#include <DShow.h>
#include <string>
#include <Ks.h>
#include <KsMedia.h>
#include <stdio.h>



// export functions
extern "C" PRINCEDLL_API HRESULT APIENTRY InitLibrary();
extern "C" PRINCEDLL_API HRESULT APIENTRY SetFaceTracking ( long boolValue );
extern "C" PRINCEDLL_API HRESULT APIENTRY GetFaceTracking ( long *boolValue );