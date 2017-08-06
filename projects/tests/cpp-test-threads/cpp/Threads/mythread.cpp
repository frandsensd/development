#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <synchapi.h>
#include "mythread.h"

static char* szPriority[MyThread::PRIORITY_NUMBER_OF_PRIORITIES] =
{
	"PRIORITY_DEFAULT",
	"PRIORITY_LOW",
	"PRIORITY_HIGH",
	"PRIORITY_ISR"
};


MyThread::MyThread(MyThread::threadCreationArgs creationArgs)
{
	std::cout << "MyThread::MyThread" << std::endl;
	if (RES_ERROR == Create(creationArgs)) {
		std::cout << "Error: Failed creating thread " << creationArgs.name << std::endl;
	}
}

MyThread::MyThread(threadCreationArgs creationArgs, const MyThread::ePriority tprio = MyThread::PRIORITY_DEFAULT, const size_t stackSize = DEFAULT_STACKSIZE)
{
	std::cout << "MyThread::MyThread(args)" << std::endl;
	if (RES_ERROR == Create(creationArgs, tprio, stackSize)) {
		std::cout << "Error: Failed creating thread " << creationArgs.name << std::endl;
	}
}

MyThread::~MyThread()
{
	std::cout << "MyThread::~MyThread" << std::endl;
	std::vector<MyThread::eventInfo>::iterator it;
	UINT i = 1;
	
	do
	{
		it = _threadEvents.info.begin();
		std::cout << "Delete event(" << i++ << ") name: " << it->name << std::endl;
		free( (PVOID)it->name );
#if defined (_WIN32) || defined (_WIN64)
		CloseHandle(it->handle);
#else
#endif
		_threadEvents.info.erase(it);
	} while (!_threadEvents.info.empty());
	std::cout << "events left in vector: " << _threadEvents.info.size() << std::endl;
}

MyThread::eRes MyThread::Create(threadCreationArgs creationArgs)
{
	std::cout << "MyThread::Create default thread" << std::endl;
	if (NULL == creationArgs.lpStartAddress) {
		return RES_ERROR;
	}
	if (0 == creationArgs.name.length()) {
		creationArgs.name.assign("default");
	}
	return Create(creationArgs, PRIORITY_DEFAULT, DEFAULT_STACKSIZE);
}


MyThread::eRes MyThread::Create(threadCreationArgs creationArgs, const MyThread::ePriority tprio, const size_t stackSize = DEFAULT_STACKSIZE)
{
#if defined (_WIN32) || defined (_WIN64)
	_name = creationArgs.name;
	_priority = tprio;
	_args.context = this;
	_args.lpParameter = creationArgs.lpParameter;
	_handle = CreateThread(
		_In_opt_	NULL,	// thread cannot be inherited, LPSECURITY_ATTRIBUTES = null
		_In_		stackSize,
		_In_       creationArgs.lpStartAddress,
		_In_opt_   &_args,
		_In_       CREATE_SUSPENDED,	// 0=run immediately, CREATE_SUSPENDED requiring ResumeThread for starting
		_Out_opt_  _ID
		);
	SetThreadPriority(_handle, GetSystemSpecificThreadPriorityFromThreadPriority());
	std::cout << "MyThread::Create " << _name << " with priority " << szPriority[_priority] << " - got handle " << std::hex << _handle << std::endl;
	if (_handle) {
		return RES_OK;
	}
	else {
		return RES_ERROR;
	}
#else
	return RES_NOT_IMPLEMENTED;
#endif
}

MyThread::eRes MyThread::Start()
{
	std::cout << "MyThread::Start " << _name << " with handle " << std::hex << _handle << std::endl;
#if defined (_WIN32) || defined (_WIN64)
	DWORD res = ResumeThread(_handle);
	if (-1 == res) {
		std::cerr << "ERROR: Cannot start thread - failed with error: " << GetLastError() << std::endl;
		return RES_ERROR;
	}
	return RES_OK;
#else
	return RES_NOT_IMPLEMENTED;
#endif
}

MyThread::eRes MyThread::Terminate()
{
	std::cout << "MyThread::Terminate" << std::endl;
	return RES_OK;
}

#define MAX_EVENTNAME_CHARS	256
MyThread::eRes MyThread::DeclareEvents(eventArgsVector& aEventArgsVector)
{
	std::cout << "MyThread::DeclareEvents" << std::endl;
#if defined (_WIN32) || defined (_WIN64)
	std::vector<eventArgs>::iterator iEventArgs;
	UINT i = 1;
	for (iEventArgs = aEventArgsVector.args.begin(); iEventArgs < aEventArgsVector.args.end(); ++iEventArgs)
	{
		eventInfo tmp;
		tmp.handle = CreateEvent(
			iEventArgs->lpEventAttributes,
			iEventArgs->bManualReset,
			iEventArgs->bInitialState,
			iEventArgs->lpName );
		if (tmp.handle) {
			std::cout << "Event(" << i++ << ") name:" << iEventArgs->lpName << " added\n";
			size_t len = strnlen_s(iEventArgs->lpName, MAX_EVENTNAME_CHARS) + 1;
			tmp.name = (LPCTSTR) malloc(len * sizeof(CHAR));
			if (tmp.name) {
				strncpy_s((CHAR*)tmp.name, len, (CHAR*)iEventArgs->lpName, _TRUNCATE);
			}
			else {
				std::cout << "ERROR: Out of memory!" << std::endl;
				CloseHandle(tmp.handle);
				return RES_OUT_OF_MEMORY;
			}
			_threadEvents.info.push_back(tmp);
		}
		else {
			std::cout << "ERROR: Failed adding event(" << i++ << ") name: " << *iEventArgs->lpName << std::endl;
		}
	}
	/* Just for debugging - show all events in vector:
	std::vector<eventInfo>::iterator iEventInfo;
	i = 1;
	for (iEventInfo = _threadEvents.info.begin(); iEventInfo < _threadEvents.info.end(); ++iEventInfo)
	{
		std::cout << "event(" << i++ << ") name: " << iEventInfo->name << std::endl;
	}
	*/
	return RES_OK;
#else
	return RES_NOT_IMPLEMENTED;
#endif
}

#define MAX_EVENT_NAME_CHARS	50
HANDLE MyThread::GetEventHandle(const LPCTSTR name)
{
	std::vector<eventInfo>::iterator it;
	for (it = _threadEvents.info.begin(); it < _threadEvents.info.end(); ++it)
	{
		if (!strncmp(it->name, name, strnlen(it->name,MAX_EVENT_NAME_CHARS)))
			return it->handle;
	}
	return NULL;
}

INT	MyThread::GetSystemSpecificThreadPriorityFromThreadPriority()
{
	if (PRIORITY_LOW == _priority) return THREAD_PRIORITY_BELOW_NORMAL;
	else if (PRIORITY_HIGH == _priority) return THREAD_PRIORITY_ABOVE_NORMAL;
	else if (PRIORITY_ISR == _priority) return THREAD_PRIORITY_TIME_CRITICAL;
	else return THREAD_PRIORITY_NORMAL;
}

MyThread::eRes MyThread::WaitForEvent(const LPCTSTR name)
{
	std::cout << "MyThread::WaitForEvent: " << name << std::endl;
	HANDLE h = GetEventHandle(name);
	if (!h)
		return RES_ERROR;
#if defined (_WIN32) || defined (_WIN64)
	DWORD res = WaitForSingleObject(h, 1000);// [ms / INFINITE]
	switch (res)
	{
	case WAIT_ABANDONED:
		std::cerr << "ERROR: Wait abandoned!\n";
		return RES_ERROR;
	case WAIT_OBJECT_0:
		return RES_OK;
	case WAIT_TIMEOUT:
		std::cerr << "WARN: Wait timed out!\n";
		return RES_ERROR;
	case WAIT_FAILED:
		std::cerr << "ERROR: Failed with code " << GetLastError() << std::endl;
		return RES_ERROR;
	default:
		std::cerr << "ERROR: Unexpected response from WaitForSingleObject" << std::endl;
		return RES_ERROR;
	}
#else
	return RES_NOT_IMPLEMENTED;
#endif
}

MyThread::eRes MyThread::SendEvent(const LPCTSTR name)
{
	std::cout << "MyThread::SetEvent: " << name << std::endl;
	HANDLE h = GetEventHandle(name);
	if (!h) {
		return RES_ERROR;
	}
#if defined (_WIN32) || defined (_WIN64)
	if (SetEvent((HANDLE)h)) {
		return RES_OK;
	}
	else {
		std::cerr << "ERROR: Failed with code: " << GetLastError() << std::endl;
		return RES_ERROR;
	}
	return RES_OK;
#else
	return RES_NOT_IMPLEMENTED;
#endif
}

MyThread::eRes MyThread::ClearEvent(const LPCTSTR name)
{
	std::cout << "MyThread::ClearEvent: " << name << std::endl;
	HANDLE h = GetEventHandle(name);
	if (!h) {
		return RES_ERROR;
	}
#if defined (_WIN32) || defined (_WIN64)
	if (ResetEvent(h)) {
		return RES_OK;
	}
	else {
		std::cerr << "ERROR: Failed with code: " << GetLastError() << std::endl;
		return RES_ERROR;
	}
#else
	return RES_NOT_IMPLEMENTED;
#endif
}
