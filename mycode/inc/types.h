#ifndef MYTYPES_H
#define MYTYPES_H

//
// GENERIC TYPES
//
typedef unsigned char                   UINT8;
typedef unsigned short                  UINT16;
typedef unsigned int                    UINT32;
typedef unsigned long long              UINT64;

typedef char                            INT8;
typedef short                           INT16;
typedef int                             INT32;
typedef long long                       INT64;

typedef void                            VOID;
typedef unsigned int                    BOOL;
typedef struct { double r; double i; }  COMPLEX;


#ifndef FALSE
#define FALSE   (BOOL)0
#endif
#ifndef TRUE
#define TRUE    !FALSE
#endif

#define MIN(a,b)    (a)<(b) ? (a) : (b)
#define MAX(a,b)    (a)>(b) ? (a) : (b)

#endif // MYTYPES_H
