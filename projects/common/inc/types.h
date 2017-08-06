#ifndef MYTYPES_H
#define MYTYPES_H

extern "C"
{
#if defined (_WIN32) || !defined(_WIN64)
	typedef double				            DBL;
#elif defined _linux
	typedef char                            INT8;
	typedef void                            VOID;
	typedef bool                            BOOL;
	typedef double				            DBL;
#endif
//
// GENERIC TYPES
//
typedef unsigned char                   UINT8;
typedef unsigned short                  UINT16;
typedef unsigned int                    UINT32;
typedef unsigned long long              UINT64;

typedef short                           INT16;
typedef int                             INT32;
typedef long long                       INT64;

typedef float				            FLT;

typedef char				            CHAR;

#ifndef FALSE
#define FALSE   (BOOL)0
#endif
#ifndef TRUE
#define TRUE    !FALSE
#endif

#define MIN(a,b)    (a)<(b) ? (a) : (b)
#define MAX(a,b)    (a)>(b) ? (a) : (b)
}
#endif // MYTYPES_H
