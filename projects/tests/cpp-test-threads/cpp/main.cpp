#include <Windows.h>
#include <iostream>
#include <string>
#include <complex>		// stdlib templated version
#include <bitset>		// stdlib for binary cout

#include <types.h>
#include "complex.h"
#include "env.h"
#include "mythread.h"
#include "BitManipulation.h"
#include "mylogger.h"

DWORD WINAPI	Thread1(LPVOID);
DWORD WINAPI	Thread2(LPVOID);


int main(void)
{
	MyLogger*	pl = MyLogger::Instance();
	pl->Log("--------------------------------------------------------------------------------\n");
	pl->Log("-------My Complex Class test\n");
	pl->Log("--------------------------------------------------------------------------------\n");
	Complex a(1000.2, -1262.3);
	Complex b(1222.1, -1744.7);
	Complex c;

	c = a.Conj();
	//std::cout << "Real(a)=" << a.Real() << " Imag(a)=" << a.Imag() << " a=" << a << " Conj(a)=" << c << std::endl;
	pl->Log( TEXT("Real(a)=%6.4f Imag(a)=%6.4f ", a.Real(), a.Imag()) );
	pl->Log( " a=" << a );
	pl->Log( " Conj(a)=" << c << std::endl );

	c = a + b;
	std::cout << "c=a+b=" << a << "+" << b << "=" << c << std::endl;
	std::cout << "a==b=" << a << "==" << b << "=" << ((a == b) ? "TRUE" : "FALSE") << std::endl;
	a = b;
	std::cout << "a==b=" << a << "==" << b << "=" << ((a == b) ? "TRUE" : "FALSE") << std::endl;

	std::cout << "a+=b=" << a << "+=" << b << "=" << (a += b) << std::endl;
	std::cout << "b+=a=" << b << "+=" << a << "=" << (b += a) << std::endl;
	// Note: In above two lines a and b are already overwritten before being dumped in this fashion.

	a = Complex(2, -3);
	b = Complex(-2, 3);
	c = a - b;
	std::cout << "c=a-b=" << a << "-" << b << "=" << c << std::endl;

	a = Complex(2.0, -3.0);
	b = Complex(-2, 3);
	std::cout << "a=" << a << "; b=" << b << std::endl;
	c = a * b;
	std::cout << "c=a*b=" << a << "*" << b << "=" << c << std::endl;

	a = Complex(4, -5);
	b = Complex(-6, 7);
	std::cout << "a=" << a << "; b=" << b << std::endl;
	c = a / b;
	std::cout << "c=a/b=" << a << "/" << b << "=" << c << std::endl;

	a = Complex(4, -5);
	b = Complex(0, 0);
	try
	{
		c = a / b;
	}
	catch (std::invalid_argument &e)
	{
		std::cerr << "Caught: " << e.what() << std::endl;
		std::cerr << "Type: " << typeid(e).name() << std::endl;
	}
	std::cout << "c=a/b=" << a << "/" << b << "=" << c << std::endl;

	a = Complex(4, -5);
	b = Complex(-6, 7);
	c = Sqrt(a) / b.Sqrt();
	std::cout << "a=" << a << "; b=" << b << std::endl;
	std::cout << "Complex:     c=Sqrt(a)/b.Sqrt()=" << Sqrt(a) << "/" << b.Sqrt() << "= +/-" << c << std::endl;

	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << "-------std::complex<DBL> verification\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	std::complex<DBL>	aa(4, -5);
	std::complex<DBL>	bb(0, 0);
	std::complex<DBL>	cc;
	try
	{
		cc = aa / bb;
	}
	catch (std::exception e)
	{
		std::cerr << "Caught from complex std: " << e.what() << std::endl;
		std::cerr << "Type: " << typeid(e).name() << std::endl;
	}
	std::cout << "cc=aa/bb=" << aa << "/" << bb << "=" << cc << std::endl;

	std::complex<DBL>	d(4, -5);
	std::complex<DBL>	e(-6, 7);
	std::complex<DBL>	f = sqrt(d) / sqrt(e);
	std::cout << "d=" << d << "; e=" << e << std::endl;
	std::cout << "std:complex: f=sqrt(d)/sqrt(e)=" << sqrt(d) << "/" << sqrt(e) << "= +/-" << f << std::endl;


	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << "-------Environment variable fun:\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	char buf[ENV_VALUE_MAX_CHARS];
	std::cout << "Hello " << GetEnv("USERNAME", buf, ENV_VALUE_MAX_CHARS) << ". ";
	std::cout << "I see you're working on " << GetEnv("COMPUTERNAME", buf, ENV_VALUE_MAX_CHARS) << std::endl;
	std::string myPath("d:\\development\\raspberrypi\\projects\\tests\\cpp-test-threads\\cpp\\");
	std::cout << "Current path=" << GetEnv("PATH", buf, ENV_VALUE_MAX_CHARS) << std::endl;
	buf[40] = ';';
	buf[41] = '\0';
	
	myPath.insert(0, buf);
	PutEnv("PATH", myPath.c_str());
	std::cout << "New path=" << GetEnv("PATH", buf, ENV_VALUE_MAX_CHARS) << std::endl;

	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << "-------Algorithm fun:\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	UINT16 num = 0x1234;
	std::cout << "num=0x" << std::hex << num << "\n = 0b" << std::bitset<16>(num);
	std::cout << ". ToggleBits(3-10) =>\n = 0b" << std::bitset<16>(ToggleBits<UINT>(3, 10, num)) << std::endl;
	num = 0x1234;
	std::cout << "num=0x" << std::hex << num << "\n = 0b" << std::bitset<16>(num);
	std::cout << ". ReverseBits(3-10) =>\n = 0b" << std::bitset<16>(ReverseBits<UINT>(3, 10, num)) << std::endl;
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << "-------Threads fun:\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	MyThread::threadCreationArgs args = { "t1", Thread1, NULL };
	MyThread	t1( args );
	MyThread::eventArgs EventArgs;
	MyThread::eventArgsVector Events;
	EventArgs.bInitialState = FALSE;		// not signalled
	EventArgs.bManualReset = FALSE;		// auto-reset
	EventArgs.lpEventAttributes = NULL;	// Default security descriptor
	EventArgs.lpName = "t1 start";
	Events.args.push_back(EventArgs);
	EventArgs.lpName = "t1 stop";
	Events.args.push_back(EventArgs);
	if (MyThread::RES_OK != t1.DeclareEvents(Events)) {
		return -1;
	}

	Events.args.clear();
	MyThread	t2({"t2", Thread2, NULL }, MyThread::PRIORITY_HIGH, 100);
	EventArgs.lpName = "t2 start";
	Events.args.push_back(EventArgs);
	EventArgs.lpName = "t2 stop";
	Events.args.push_back(EventArgs);
	if (MyThread::RES_OK != t2.DeclareEvents(Events)) {
		return -2;
	}
	Events.args.clear();

	t1.Start();
	t2.Start();
	return 0;
}

DWORD WINAPI Thread1(LPVOID apArgs)
{
	MyThread::threadArgs* pArgs = static_cast<MyThread::threadArgs*>(apArgs);
	MyThread* pThread = static_cast<MyThread*>(pArgs->context);
	std::cout << "Thread " << pThread->GetName() << "{ID,handle}={" << pThread->GetThreadID();
	std::cout << "," << std::hex << pThread->GetHandle() << "} running in context " << pArgs->context << std::endl;
	UINT16 num = 0x1234;
	std::cout << "num=0x" << std::hex << num << "\n = 0b" << std::bitset<16>(num);
	std::cout << ". ToggleBits(3-10) =>\n = 0b" << std::bitset<16>(ToggleBits<UINT16>(3, 10, num)) << std::endl;
	Sleep(2000);
	pThread->SendEvent("t2 start");
	pThread->WaitForEvent("t1 stop");
	pThread->SendEvent("t2 stop");
	return 0;
}

DWORD WINAPI Thread2(LPVOID apArgs)
{
	MyThread::threadArgs* pArgs = static_cast<MyThread::threadArgs*>(apArgs);
	MyThread* pThread = static_cast<MyThread*>(pArgs->context);
	pThread->WaitForEvent("t2 start");

	std::cout << "Thread " << pThread->GetName() << "{ID,handle}={" << pThread->GetThreadID();
	std::cout << "," << pThread->GetHandle() << "} running in context " << pArgs->context << std::endl;
	UINT32 num = 0x12345678;
	std::cout << "num=0x" << std::hex << num << "\n = 0b" << std::bitset<32>(num);
	std::cout << ". ReverseBits(8-16) =>\n = 0b" << std::bitset<32>(ReverseBits<UINT16>(8, 16, num)) << std::endl;
	pThread->SendEvent("t1 stop");
	pThread->WaitForEvent("t2 stop");
	return 0;
}
