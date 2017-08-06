#include <iostream>
#include "Calculator.h"

Calculator::Calculator()
{
}

Calculator::~Calculator()
{
}


CPLX* Calculator::Add( CPLX& a, CPLX& b)
{
	std::cout << "CPLX\n";
	CPLX* res = (CPLX*) new CPLX;
	res->r = a.r+b.r;
	res->i = a.i+b.i;
	return res;
}

// overload << operator..
std::ostream& operator<<(std::ostream& os, const CPLX& str)
{
	os << "(" << str.r << "," << str.i << ")";
	return os;
}

template <class T>
T	Calculator::Add( T a,	T b)
{
	std::cout << "T\n";
	T res = a+b;
	return res;
}


/*
UINT8 Calculator::Add( UINT8 a, UINT8 b)
{
	std::cout << "UINT8\n";
	return a+b;
}

INT8 Calculator::Add( INT8 a, INT8 b)
{
	std::cout << "INT8\n";
	return a+b;
}

UINT16 Calculator::Add( UINT16 a, UINT16 b)
{
	std::cout << "UINT16\n";
	return a+b;
}

INT16 Calculator::Add( INT16 a, INT16 b)
{
	std::cout << "INT16\n";
	return a+b;
}

UINT32 Calculator::Add( UINT32 a, UINT32 b)
{
	std::cout << "UINT32\n";
	return a+b;
}

INT32 Calculator::Add( INT32 a, INT32 b)
{
	std::cout << "INT32\n";
	return a+b;
}

FLT Calculator::Add( FLT a, FLT b)
{
	std::cout << "FLT\n";
	return a+b;
}

DBL Calculator::Add( DBL a, DBL b)
{
	std::cout << "DBL\n";
	return a+b;
}
*/
