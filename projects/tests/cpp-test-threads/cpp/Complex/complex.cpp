//
//On operator overloading, the simple canonical way is in general for comparison operators:
//inline bool operator==(const X& lhs, const X& rhs){ /* do actual comparison */ }
//inline bool operator!=(const X& lhs, const X& rhs){return !operator==(lhs,rhs);}
//inline bool operator< (const X& lhs, const X& rhs){ /* do actual comparison */ }
//inline bool operator> (const X& lhs, const X& rhs){return  operator< (rhs,lhs);}
//inline bool operator<=(const X& lhs, const X& rhs){return !operator> (lhs,rhs);}
//inline bool operator>=(const X& lhs, const X& rhs){return !operator< (lhs,rhs);}
//(Ref: http://stackoverflow.com/questions/4421706/operator-overloading/4421719#4421719)
//

#include <types.h>
#include <iostream>
#include <iomanip>	//<< std::setw(6) << std::setprecision(2) << std::setfill('0')
#include <math.h>	// sqrt()
#include "complex.h"

#ifdef _DEBUG
#define LOG(TXT)	std::cout << "Complex::" << TXT << std::endl
#else
#define LOG(TXT)	
#endif

// stream write
#define STREAM_OUT_NUMERIC_FORMAT		std::fixed << std::setprecision(3)


//template <class T>
// use DBL for now...once working, generalize by replace DBL by <T>


//Default(no args => real/imag=0) and basic copy-constructor
//Note: As this class do not require array allocation no separate copy is required.
// Otherwise we will need to use e.g. std::copy(x.ary, x.ary+size, this.ary)..
Complex::Complex(const DBL real, const DBL imag)
	: r(real), i(imag)
{
	LOG("Complex(r,i)");
}

//Copy-constructor
Complex::Complex(const Complex& x)
	: r(x.r), i(x.i)
{
	LOG("Complex(x)");
}

Complex::~Complex()
{
	LOG("~Complex");
}

// Required for copy-swap idiom (more efficient assignment operator overload!)
void	Complex::Swap(Complex& x, Complex& y)
{
	LOG("Swap");
	std::swap(x.r, y.r);
	std::swap(x.i, y.i);
}

DBL	Complex::Real()
{
	LOG("Real");
	return r;
}

DBL	Complex::Imag()
{
	LOG("Imag");
	return i;
}

Complex Complex::Conj()
{
	LOG("Conj");
	Complex c(*this);
	c.i = -c.i;
	return c;
}

/*           +         a + Sqrt(a^2 + b^2)                  -a + Sqrt(a^2 + b^2)
Sqrt(a+bi) =   ( Sqrt( ------------------- ) + Sgn(b) Sqrt( -------------------- )i )
             -                  2                                     2
Pre-requisite: b!=0.
If b==0, it reduces to Sqrt(a).
Note: Sgn(b) is signum function, i.e. sign(b) = (b<0) ? -1 : b(>0) ? 1 : 0;
a,b = r,i
Returning +part. Up to user to define the -part.
*/
Complex Complex::Sqrt()
{
	LOG("Sqrt");
	if (i == 0) {
		Complex res(sqrt(r), 0);
		return res;
	}
	else {
		int sign = i < 0 ? -1 : i>0 ? 1 : 0;
		Complex res(sqrt( (r + sqrt(r*r + i*i))/2 ), sign*sqrt( (sqrt(r*r + i*i) - r) / 2) );
		return res;
	}
}


///////////////////////////////////////
//
// class specific operations
//
///////////////////////////////////////
// NOTE: x passed by value on purpose - using stack frame for deallocation - more elegant!
Complex&	Complex::operator=(Complex x)
{
	LOG("=");
	Swap(*this, x);
	return *this;
}

std::ostream&	Complex::operator<<(std::ostream& os)
{
	os << "(" << STREAM_OUT_NUMERIC_FORMAT << r << "," << STREAM_OUT_NUMERIC_FORMAT << i << ")";
	return os;
}

bool	Complex::operator==(const Complex& x)
{
	LOG("==");
	if (x.r == r && x.i == i)
		return true;
	else
		return false;
}

Complex&	Complex::operator+=(const Complex& x)
{
	LOG("+=");
	r += x.r;
	i += x.i;
	return *this;
}

Complex&	Complex::operator-=(const Complex& x)
{
	LOG("-=");
	r -= x.r;
	i -= x.i;
	return *this;
}

Complex&	Complex::operator*=(const Complex& x)
{
	LOG("*=");
	if (x.r == r == 0) {
		r = -x.i * i;
		i = 0; // cross products always zero if pure real numbers
	}
	else if (x.i == i == 0) {
		r = x.r * r;
		// cross products always zero if pure imag numbers
	}
	else {
		r = x.r * r - (x.i*i);	// real product - imag product
		i = x.r * i + x.i * r;	// sum of cross products
	}
	return *this;
}

/*
            Conj(c+di)
			   |
a+bi   (a+bi)(c-di)    (ac - bd*(i^2))        (bc - ad)         (ac+bd) + i(bc-ad)
---- = ------------ = ----------------- + i----------------- = -------------------
c+di   (c+di)(c-di)   (c^2 - d^2*(i^2))    (c^2 - d^2*(i^2))        c^2 + d^2
a,b = r,i
c,d = x.r,x.i
*/
Complex&	Complex::operator/=(const Complex& x)
{
	LOG("/=");
	DBL	denum(x.r*x.r + x.i*x.i); // Real number as imaginary parts cancel
	Complex num(r*x.r + i*x.i, i*x.r - r*x.i);
	if (0 == denum) {
		// of denum = 0 => num = 0 => 0/0 = NAN!
		r = -NAN;
		i = -NAN;
//		throw InvalidParameterException();
		return *this;
	}
	r = num.Real() / denum;
	i = num.Imag() / denum;
	return *this;
}



///////////////////////////////////////
//
// Non-class specific friend operations
//
///////////////////////////////////////
std::ostream&	operator<<(std::ostream& os, const Complex x)
{
	os << "(" << STREAM_OUT_NUMERIC_FORMAT << x.r << "," << STREAM_OUT_NUMERIC_FORMAT << x.i << ")";
	return os;
}

/* stream read: Problem: Need to define how to determine whether is contains valid Complex object...
std::istream&	operator>>(std::istream& is, Complex& x)
{
	if ( 0 )
	{
		// no valid x obj of type Complex found in stream
		is.setstate(std::ios::failbit);
	}
	else {
		is >> x;
	}
	return is;
} */

bool	operator==(const Complex& x, const Complex& y)
{
	LOG("x==y");
	if (x.r == y.r && x.i == y.i)
		return true;
	else
		return false;
}

/* Straight forward 
Complex		operator+(const Complex& x, const Complex& y)
{
	LOG("+v1");
	Complex res;
	res.r = x.r + y.r;
	res.i = x.i + y.i;
	return res;
}*/

/* Smart: x is pushed on stack at call, y passed by ref.
** Reusing += operator overload, we now overwrite x on stack and pass stack frame as result.
** No cleanup required and very simple implementation :) */
Complex		operator+(Complex x, const Complex& y)
{
	LOG("+v2");
	x += y;
	return x;
}

Complex		operator-(Complex x, const Complex& y)
{
	LOG("-");
	x -= y;
	return x;
}

Complex		operator*(Complex x, const Complex& y)
{
	LOG("*");
	x *= y;
	return x;
}

Complex		operator/(Complex x, const Complex& y)
{
	LOG("/");
	x /= y;
	return x;
}

Complex		Sqrt(Complex& x)
{
	LOG("Sqrt(x)");
	return x.Sqrt();
}
