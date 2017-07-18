#pragma once

#if defined ADCDRIVER_DLL
#define API __declspec(dllexport) 
#else
#define API __declspec(dllimport) 
#endif
