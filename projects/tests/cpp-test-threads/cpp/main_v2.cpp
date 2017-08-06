#include <iostream>
#include <types.h>
//#include "Calculator.h"


class CPLX
{
public:
	CPLX(DBL real, DBL imag):r(real),i(imag){};
	CPLX operator+(const CPLX& b) const
	{
		return CPLX(r+b.r, i+b.i);
	}
	std::ostream& operator<<(std::ostream& os)
	{
		os << "(" << r << "," << i << ")";
		return os;
	}
private:
	DBL r,i;
};

template <class T>
T sum (T a, T b) { return a+b; }



int main(void)
{
	std::cout << "res=" << sum<UINT16>(2,3) << std::endl;
	std::cout << "res=" << sum<FLT>(2.33,3.34) << std::endl;
	CPLX ca(1,-3);
	CPLX cb(2,3);
	std::cout << "res=" << sum<CPLX>(ca, cb) << std::endl;
	return 0;
}
