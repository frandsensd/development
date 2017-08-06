// Compile in RASPBIAN as 
// g++ main.cpp -o launchme -lstdc++ -pthread -lrt
//
// NOTE: This code is _NOT_ supporting C++ 2011 or later standards
//

#if __cplusplus == 1
#warning "Compiling for C++ pre-1998" 
#elif __cplusplus == 19971L
#warning "Compiling for C++ 1998" 
#elif __cplusplus == 201103L
#warning "Compiling for C++ 2011"
#elif __cplusplus == 201402L
#warning "Compiling for C++ 2014" 
#else
#warning "Compiling for unknown C++ standard" 
#endif

//using namespace std;	// make life easier for cout, endl etc..


//
// INCLUDES
//
#include <iostream>


// Thread support
#include <pthread.h>


// Process state change wait, waitpid, waitid
#include <sys/types.h>
#include <sys/wait.h>



// Semaphore handling
// Unnamed semaphore: see sem_init, sem_wait, sem_trywait, sem_timedwait, sem_post
// Named semaphore: see sem_open, sem_wait, sem_trywait, sem_timedwait, sem_post, sem_close
#include <semaphore.h>


// Signalling; signal, raise -- deprecated!!!
// void ( *signal(int sig, void (*func)(int)) ) (int);
// NOTE: Unknown behavior if used in multithreaded environment... Does not throw exceptions.
// int raise (int sig);
// Typical sig values: SIGINT=normal indication, SIGTERM=terminate, ...
// ex:	void Handler(int param){ ... }
//	void (*PrevHandler)(int);
//	PrevHandler = signal (SIGINT, Handler);
//	raise (SIGINT);
//
// BETTER alternative used in Linux:
// #include <signal.h>
// int sigaction(int sig, const struct sigaction *newAction, struct sigaction *oldAction);
// ex:	void Handler(int param){ ... }
// 	struct sigaction	act;
// 	act.sa_handler = Handler;
//	siginfo_t		sInfo;
// 	sigemptyset(&act.sa_mask);	// no signals to be masked
// 	act.sa_flags = SA_RESETHAND	// required to auto-reset signal when using sigaction.
//                     |SA_SIGINFO;	// required for additional info
// 	if (sigaction(SIGINT, &act, 0))	// disregard saving old action
//      {   handleErrorCase(errno)   }
#include <csignal>



#include <assert.h> 		// define NDEBUG prior to including this to avoid code generation for asserts.
#include <cerrno>
#include "types.h"


//
// DEFINES
//
#define OK			0
#define SIGNALS_PRODUCED	5


//
// TYPEDEFS
//
typedef struct _CONTEXT
{
	UINT32	signalsSent;
	UINT32	signalsReceived;
	BOOL	isSignalled;
}CONTEXT, *PCONTEXT;




//
// GLOBALS/LOCALS
//
static sem_t	sCriticalSection;	// <semaphore.h>
static CONTEXT	sContext;


//
// FUNCTIONS
//

// Common signal handler for threads - must be mutex protected
//static void SignalHandler(int signo, siginfo_t* psi=NULL, void* pctxt=NULL)
static void SignalHandler(int signo)
{
	int res;

	res = sem_wait(&sCriticalSection); // lock semaphore
	// note: sem_trywait(&sCriticalSection); returns immediately
	if (-1 == res) {
		std::cout << "Error: SignalHandler cannot retrieve semaphore!\n";
		return;
	}

	std::cout << ">SignalHandler, signo=" << signo << std::endl;
/*	if (pctxt) {
		ucontext_t* pContext = static_cast<ucontext_t*>(pctxt);
		std::cout << "we have a context..\n";
		// analyze pContext...
	}
*/
	sContext.isSignalled = true;

	res = sem_post(&sCriticalSection); // unlock semaphore
	if (-1 == res) {
		std::cout << "Error: Cannot unlock semaphore!\n";
	}
	std::cout << "<SignalHandler\n";
}



void* ProducerThread(void* pContext)
{
	PCONTEXT pctxt = static_cast<PCONTEXT>(pContext);
	std::cout << ">Producer[" << getpid() << "], pctxt=" << pContext << std::endl;
	int res;
	clockid_t timerclockid;
	pid_t pid;

	// Define signal
	struct sigaction act;
	act.sa_handler = SignalHandler;
	act.sa_flags = SA_RESETHAND|SA_SIGINFO;

	// Send
	int i=1;
	do
	{
		std::cout << "loop iteration#" << i << std::endl;
		// From Windows world this is similar to SetEvent/KeSetEvent
		if ( sigaction( SIGINT, &act, NULL ) == -1 )
		{
			std::cout << "Error: sigaction failed with error " << errno << std::endl;
		}
		else
		{
			std::cout << "Signal " << i << " sent\n";
			if (pctxt) {
				pctxt->signalsSent++;
			}
			else {
				std::cout << "Invalid pContext - using local static!\n";
				sContext.signalsSent++;
			}
		}
		usleep(1*1000*1000); // [us]
	} while (i++ < SIGNALS_PRODUCED);
	std::cout << "<Producer\n";
}


void* ConsumerThread(void* pContext)
{
	PCONTEXT pctxt = static_cast<PCONTEXT>(pContext);
	std::cout << ">Consumer[" << getpid() << "], pctxt=" << pContext << std::endl;
	// Wait forever on signal from producer or kill signal or timeout.


	// if producer signal SIGINT => clear isSignalled and increment signalsReceived
	UINT32 signalsReceived;
	do
	{
		std::cout << ".";

		//http://linux.die.net/man/2/waitid
		//int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
		//idtype_t {P_PID (processID=id), P_PGID (processGroupID=id), P_ALL (id is ignored)}
		//id_t {}
		//siginfo_t {si_pid,si_uid=?,si_signo=SIGCHLD,si_status,si_code,
		//options {WEXITED|WSTOPPED|WCONTINUED|WNOHANG|WNOWAIT}
		siginfo_t siginfo;
		int res = waitid( P_ALL, 0, &siginfo, WSTOPPED );
		if (-1 == res) {
			std::cout << "waitid failed with code " << errno << std::endl;
		}
		else {
			std::cout << "received signal from process ID " << res << std::endl;
			std::cout << "reason: ";
			std::cout << static_cast<const char*>(
				(siginfo.si_code & CLD_STOPPED) ? "CLD_STOPPED"
				:(siginfo.si_code & CLD_EXITED) ? "CLD_EXITED"
				:(siginfo.si_code & CLD_DUMPED) ? "CLD_DUMPED"
				:(siginfo.si_code & CLD_KILLED) ? "CLD_KILLED"
				:(siginfo.si_code & CLD_TRAPPED) ? "CLD_TRAPPED"
				:"CLD_CONTINUED") << std::endl;
		}
		if (pctxt) {
			pctxt->isSignalled = false;
			signalsReceived = ++pctxt->signalsReceived;
		}
		else {
			std::cout << "pContext NULL - referencing local static!\n";
			sContext.isSignalled = false;
			signalsReceived = ++sContext.signalsReceived;
		}

		std::cout << ":";
	} while (signalsReceived < SIGNALS_PRODUCED);

	// if kill/timeout => exit thread.
	std::cout << "<Consumer\n";
}


int main(int argc, char* argv[])
{
	std::cout << "Hello World\n";
	for (int i=0; i<argc; i++)
		std::cout << "arg[i]=" << argv[i] << std::endl;	// argv guaranteed to be valid (at least:command itself)

	// Thread vars
	int 			res;
	pthread_t 		hProducerThread;
	pthread_t 		hConsumerThread;
	pthread_attr_t 		attr;

	// Context initialization
	sContext.signalsSent = 0;
	sContext.signalsReceived = 0;
	sContext.isSignalled = false;

	// Init thread attr
	res = pthread_attr_init(&attr); // default stack: 2MB, call pthread_attr_destroy when unused
	assert(OK == res);

	// Init semaphore
	res = sem_init(&sCriticalSection, 0 /* shared among threads within process */, 1 /* >0 => lockable */);
	if (-1 == res ) {
		std::cout << "Error: Cannot initialize semaphore!\n";
	}

	std::cout << "Creating consumer thread\n";
	res = pthread_create(&hConsumerThread, &attr, ConsumerThread, (void*)&sContext);
	assert(OK == res);

	std::cout << "Creating producer thread\n";
	res = pthread_create(&hProducerThread, &attr, ProducerThread, (void*)&sContext);
	assert(OK == res);

	// Once threads are created we don't need attr anymore
	res = pthread_attr_destroy(&attr);
	assert(OK == res);
	
	// wait for threads to complete
	// for now just wait 5 secs
	siginfo_t siginfo;
	res = waitid( P_ALL, 0, &siginfo, WEXITED|WSTOPPED );
	if (-1 == res) {
		std::cout << "main waitid failed with code " << errno << "=";
		if (ECHILD == errno) { std::cout << "ECHILD\n"; }
		else if (EINTR == errno) { std::cout << "EINTR\n"; }
		else if (EINVAL == errno) { std::cout << "EINVAL\n"; }
		else { std::cout << "Unexpected\n"; }
	}
	else {
		std::cout << "rxd signal from process ID " << res << std::endl;
		std::cout << "reason: ";
		std::cout << static_cast<const char*>(
			(siginfo.si_code & CLD_STOPPED) ? "CLD_STOPPED"
			:(siginfo.si_code & CLD_EXITED) ? "CLD_EXITED"
			:(siginfo.si_code & CLD_DUMPED) ? "CLD_DUMPED"
			:(siginfo.si_code & CLD_KILLED) ? "CLD_KILLED"
			:(siginfo.si_code & CLD_TRAPPED) ? "CLD_TRAPPED"
			:"CLD_CONTINUED") << std::endl;
	}
	usleep( 5*1000*1000 ); // [us]
	
	std::cout << "main status:\n";
	std::cout << "signalsSent = " << sContext.signalsSent << std::endl;
	std::cout << "signalsReceived = " << sContext.signalsReceived << std::endl;
	std::cout << "isSignalled = " << sContext.isSignalled << std::endl;
	std::cout << "End of main\n";
	return 0;
}
