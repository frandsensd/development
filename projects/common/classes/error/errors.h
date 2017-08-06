#pragma once

/* Defines MACROS:
_FNENTRY
_FNEXIT
_ERRLOG(text)
_LOG(text, data)
_PRESENT_REVISION
_PRESENT_COMPILER_INFO
GETTIME
*/



/*
 * TRACE FUNCTIONALITY FLAGS
 */
//#define __SHOW_ENTRY_EXIT__
//#define __SHOW_INFO__
#define __SHOW_ERROR_INFO__



#include <iostream>
#include <ctime>
#include <types.h>

//
// GENERIC ERROR INFO
//
#define RESTXT_MAX_DESCRIPTION_CHARS    100
typedef enum
{
    RES_OK,
    RES_OUT_OF_MEMORY,
    RES_INVALID_POINTER,
    RES_INVALID_ARGUMENT,
    RES_VALUE_NOT_FOUND,
    RES_NO_DATA,
    RES_CORRUPTED_DATA,
    RES_UNSUPPORTED_PARAMETER,
    RES_NOT_IMPLEMENTED,
    RES_LAST
} RES;
extern char RESTXT[RES_LAST][RESTXT_MAX_DESCRIPTION_CHARS-1];

#define OK(res)     (RES_OK == (RES)res) ? TRUE : FALSE

//  __DATE__, __TIME__ ties to compile time.
//  __FUNCTION__, __FILE__, __LINE__ useful.

#define _PRESENT_REVISION       std::cout << __FILE__ << " built " << __DATE__ << "@" << __TIME__ << std::endl

#ifdef __cplusplus
#if __cplusplus == 199711L // pre-C++11 compiler
#define _PRESENT_COMPILER_INFO  std::cout << "Pre-C++ 11 compiler v." << __VERSION__ << " in use." << std::endl
#include <ctime>
#elif __cplusplus == 201103L // C++11 compiler
#define _PRESENT_COMPILER_INFO  std::cout << "C++ 11 compiler v." << __VERSION__ << " in use." << std::endl
#include <ctime>
#include <chrono>
#define GETTIME()   std::put_time(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now(), "%c %Z")
#else // any C++ compiler
#define _PRESENT_COMPILER_INFO  std::cout << "C++ compiler v." << __VERSION__ << " in use." << std::endl
#include <ctime>
#endif
#else
#define _PRESENT_COMPILER_INFO  std::cout << "ISO Standard C: " << __STDC__<< " Revision: " << __STDC_VERSION__ << ". C++: " << __cplusplus << std::endl
#include <ctime>
#endif

#define GETTIME std::time(NULL)
#ifdef __SHOW_ENTRY_EXIT__
#define _FNENTRY    std::cout << __FUNCTION__ << "+" << std::endl
#define _FNEXIT     std::cout << __FUNCTION__ << "-" << std::endl
#else
#define _FNENTRY
#define _FNEXIT
#endif

#ifdef __SHOW_ERROR_INFO__
#define _ERRLOG(text)  std::cout << __FUNCTION__ << "@" << __LINE__ << "ERROR:" << text << std::endl
#else
#define _ERRLOG(text)
#endif

#ifdef __SHOW_INFO__
#define _LOG(text, data)  std::cout << __FUNCTION__ << " " << "INFO:" << text << data << std::endl
#else
#define _LOG(text, data)
#endif
