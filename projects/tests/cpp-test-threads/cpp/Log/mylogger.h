#pragma once
/*
Singleton pattern:
Only one instance of logger shall be available across ALL modules using it.
instance is available by using GetLogger()
*/

#include <Windows.h>
#include <ostream>
#include <mutex>

class MyLogger
{
public:
	static MyLogger* Instance();
	void Log(LPCTSTR);
	void Log(std::ostream*);
	void LogErr(LPCTSTR);
protected:
	MyLogger();
	~MyLogger();
private:
	static MyLogger*	_instance;
	std::mutex			_mutex;
	std::ostream*		_os;
};
