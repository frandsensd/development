#pragma once
#include <types.h>
#include <iostream>
#include <stdexcept>	// template for std::invalid_argument - used to define exception
#include <string>

//template<class T>
// use DBL for now...once working, generalize by replace DBL by <T>
// compare to results using <complex> library.


// Complex divide-by-zero exception:
class InvalidParameterException : public std::invalid_argument
{
public:
	InvalidParameterException() 
		: std::invalid_argument("Invalid Parameter - causing divide-by-zero exception in Complex division!"){};
};


class Complex : public InvalidParameterException
{
public:
	Complex(const DBL real = 0, const DBL imag = 0);
	Complex(const Complex& x);
	~Complex();
	DBL		Real();
	DBL		Imag();
	void	Swap(Complex& x, Complex& y);
	Complex	Conj();
	Complex Sqrt();				// NOTE: Result is also the negated returned value (both +/- Complex)

	// Non-class specific operations
	friend std::ostream&	operator<<(std::ostream& os, const Complex x);
	//friend std::istream&	operator>>(std::istream& is, Complex& x);
	friend Complex			operator+(Complex x, const Complex& y);
	friend Complex			operator-(Complex x, const Complex& y);
	friend Complex			operator*(Complex x, const Complex& y);
	friend Complex			operator/(Complex x, const Complex& y);
	friend Complex			Sqrt(Complex& x);	// NOTE: Result is also the negated returned value (both +/- Complex)
	friend bool				operator==(const Complex& x, const Complex& y);

	// Class specific operations
	std::ostream&			operator<<(std::ostream& os);
	Complex&				operator=(const Complex x);
	Complex&				operator+=(const Complex& x);
	Complex&				operator-=(const Complex& x);
	Complex&				operator*=(const Complex& x);
	Complex&				operator/=(const Complex& x);
	bool					operator==(const Complex& x);

/*	Complex&	operator[](const UINT32 idx);			*/
/*	const Complex&	operator[](const UINT32 idx) const;		*/

protected:
private:
	DBL	r, i;
};
