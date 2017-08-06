#pragma once

// Thread Helper Class
// Create framework for creating/terminating/synchronizing threads
// Mutex handling not included.

#if defined (_WIN32) || defined (_WIN64)
#include <Windows.h>
#elif defined (_linux)
...
#endif

#include <types.h>
#include <string>
#include <vector>
#define DEFAULT_STACKSIZE	(size_t)1024*1024

class MyThread
{
public:
	// TODO: Study Linux thread priority and assign accordingly.
	// For now this is simply just defining the set to use
	typedef enum ePriority {
		#if defined (_WIN32) || defined (_WIN64)
		PRIORITY_DEFAULT = 0,	//THREAD_PRIORITY_NORMAL,
		PRIORITY_LOW,			//THREAD_PRIORITY_BELOW_NORMAL,
		PRIORITY_HIGH,			//THREAD_PRIORITY_ABOVE_NORMAL,
		PRIORITY_ISR,			//THREAD_PRIORITY_TIME_CRITICAL,
		PRIORITY_NUMBER_OF_PRIORITIES
		#endif
	};
	typedef enum eRes {
		RES_OK,
		RES_ERROR,
		RES_OUT_OF_MEMORY,
		RES_NOT_IMPLEMENTED
	};
	struct threadCreationArgs {
		std::string				name;			// name assigned to thread
		LPTHREAD_START_ROUTINE	lpStartAddress;	// start routing fn ptr
		LPVOID					lpParameter;	// arg for start routine
	};
	struct threadArgs {
		MyThread*				context;
		LPVOID					lpParameter;
	};
	struct eventArgs {
		LPCTSTR					lpName;
		LPSECURITY_ATTRIBUTES	lpEventAttributes;
		BOOL					bManualReset;
		BOOL					bInitialState;
	};
	struct eventArgsVector {
		std::vector<eventArgs>	args;
	};
	typedef struct eventInfo {
		HANDLE					handle;
		LPCTSTR					name;
	};
	typedef struct __threadEvents {
		std::vector<eventInfo>	info;
	} threadEvents;
	MyThread(threadCreationArgs creationArgs);
	MyThread(threadCreationArgs creationArgs, const MyThread::ePriority tprio, const size_t stackSize);
	~MyThread();
	eRes Terminate();
	eRes Start();
	eRes DeclareEvents(eventArgsVector& aEventArgsVector);
	eRes WaitForEvent(const LPCTSTR name);
	eRes SendEvent(const LPCTSTR name);
	eRes ClearEvent(const LPCTSTR name);
	std::string GetName() { return _name; }
	threadArgs	GetArgs() { return _args; }
	HANDLE		GetHandle() { return _handle; }
	LPDWORD		GetThreadID() { return _ID;  }
protected:
	eRes Create(threadCreationArgs creationArgs);
	eRes Create(threadCreationArgs creationArgs, const ePriority tprio, const size_t stackSize);
	HANDLE GetEventHandle(const LPCTSTR name);
	INT	GetSystemSpecificThreadPriorityFromThreadPriority();
private:
	std::string		_name;
	ePriority		_priority;
	threadArgs		_args;
	HANDLE			_handle;
	LPDWORD			_ID;
	threadEvents	_threadEvents;
};
