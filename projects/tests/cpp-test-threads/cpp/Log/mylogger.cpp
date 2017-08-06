#include <Windows.h>
#include <iostream>
#include <mutex>
#include "mylogger.h"

MyLogger* MyLogger::_instance = 0;


MyLogger* MyLogger::Instance()
{
	if (_instance == 0) {
		_instance = new MyLogger;	// The one and only instance
	}
	return _instance;
}

MyLogger::MyLogger()
{
}

MyLogger::~MyLogger()
{
	if (_instance) {
		delete _instance;
		_instance = NULL;
	}
}

void MyLogger::Log(LPCTSTR str)
{
	_mutex.lock();
	std::cout << str;
	_mutex.unlock();
}

void MyLogger::Log(std::ostream* str)
{
	_mutex.lock();
	std::cout << str;
	_mutex.unlock();
}

void MyLogger::LogErr(LPCTSTR str)
{
	_mutex.lock();
	std::cerr << str;
	_mutex.unlock();
}
