/*
** Main function will spawn threads to calculate individual results.
** Threads update the context with the results indexed by the thread id's.
**
**
**
**
*/
#pragma once
#include <types.h>

//
//
//
/*
class Calculator_base
{
public:
	Calculator_base();
	~Calculator_base();
	template<class T>
	T Add( T, T );
protected:	// inheritable by derivatives
private:
};
*/

class Calculator
{
public:
	Calculator();
	~Calculator();
	template <class T>
	T	Add( T a,	T b);
/*
	UINT8	Add( UINT8 a,	UINT8 b);
	INT8	Add( INT8 a,	INT8 b);
	UINT16	Add( UINT16 a,	UINT16 b);
	INT16	Add( INT16 a,	INT16 b);
	UINT32	Add( UINT32 a,	UINT32 b);
	INT32	Add( INT32 a,	INT32 b);
	UINT64	Add( UINT64 a,	UINT64 b);
	INT64	Add( INT64 a,	INT64 b);
	FLT	Add( FLT a,	FLT b);
	DBL 	Add( DBL a,	DBL b);
*/
	CPLX*	Add( CPLX& a,	CPLX& b);
	VOID	ShowCplxRes();
private:
	CPLX	res;
};
