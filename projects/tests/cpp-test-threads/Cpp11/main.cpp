// Compile in RASPBIAN as 
// g++ main.cpp -o launchme -lstdc++ -pthread -lrt
#if __cplusplus == 201103L
#warning "Compiling for C++ 2011" 
#elif __cplusplus == 201402L
#warning "Compiling for C++ 2014" 
#else
#error "Compiling for unsupported C++ standard"
#endif

using namespace std;
#include <iostream>

// Thread support
#include <thread>	// C++11, std::thread


// Semaphore handling
#include <mutex>	// C++11, std::mutex cs, cs.lock, cs.unlock


// Signalling; signal, raise -- deprecated!!!
#include <csignal>		// signals
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

//#include <time.h>		// timer. link with -lrt
//#include <sys/types.h>		// getpid()/getppid()
//#include <unistd.h>		// getpid()/getppid()

#include <assert.h> 		// define NDEBUG prior to including this to avoid code generation for asserts.
#include "types.h"


#define OK	(int)0L


typedef struct _CONTEXT
{
	UINT32	signalsSent;
	UINT32	signalsReceived;
}CONTEXT, *PCONTEXT;


std::mutex	cs;	// <mutex>


// Common signal handler for threads - must be mutex protected
static void SignalHandler(int signo, siginfo_t* psi=NULL, void* pctxt=NULL)
{
	cs.lock();
	if (pctxt) {
		ucontext_t* pContext = static_cast<ucontext_t*>(pctxt);
		// analyze pContext...
	}
	cout << ">SignalHandler, signo=" << signo << endl;
	cs.unlock();
	cout << "<SignalHandler" << endl;
}




void* ProducerThread(void* pContext)
{
	PCONTEXT pctxt = static_cast<PCONTEXT>(pContext);
	cout << ">Producer[" << getpid() << "], pctxt=" << pContext << endl;
	int res;
	clockid_t timerclockid;
	pid_t pid;
	struct sigaction act;
	act.sa_handler = SignalHandler;
	act.sa_flags = SA_RESETHAND|SA_SIGINFO;
	// From Windows world this is similar to SetEvent/KeSetEvent
	if ( sigaction( SIGINT, &act, NULL ) == -1 )
	{
		cout << "Error: sigaction failed with error " << errno << endl;
	}
	//timer_t timerid;
	//struct sigevent timersigevent;
	//timersigevent.sigev_notify = SIGEV_SIGNAL;
	//timersigevent.sigev_signo = SIGUSR1;
	//timersigevent.sigev_value.sival_ptr = &timerid;
	//pid = getpid();
	//res = clock_getcpuclockid( pid, &timerclockid );
	//assert(OK == res);
	//res = timer_create( timerclockid, &timersigevent, &timerid );
	//assert(OK == res);
	//cout << "producer timer created\n";
	// Start timer..
	// Wait forever on timer event or kill signal
	// - fill buffer with current time and send signal to consumer.
	// - update buffersSent
	do
	{
		int res;
		// if local timeout - treat as kill signal and terminate thread.
		//res = sigtimedwait( &sigset, &siginfo, &sigtimeout );
		//assert(OK == res);
		//res = sigaction( 
		//cout << "consumer: signal received\n";
	} while (0);
	cout << "<Producer" << endl;
}


void* ConsumerThread(void* pContext)
{
	PCONTEXT pctxt = static_cast<PCONTEXT>(pContext);
	cout << ">Consumer[" << getpid() << "], pctxt=" << pContext << endl;
	// Wait forever on signal from producer or kill signal or timeout.
	// if producer signal => consume buffer and increment buffersReceived
	// if kill/timeout => exit thread.
	cout << "<Consumer" << endl;
}


int main(int argc, char* argv[])
{
	cout << "Hello World\n";
	for (int i=0; i<argc; i++)
		cout << "arg[i]=" << argv[i] << endl;

	// Thread vars
	int 			res;
	CONTEXT			ctxt;

	// Signal vars
	//sigset_t 		sigset;
	//siginfo_t		siginfo;
	//struct timespec	sigtimeout = { 10, 0 }; // wait 10secs, 0ns, set both to 0 for polling.

	cout << "Creating consumer thread to print whatever producer sends." << endl;
	std::thread hConsumerThread (ConsumerThread, (void*)&ctxt);

	cout << "Creating producer thread sending 10 integers to consumer." << endl;
	cout << " - then sends consumer termination signal and terminates itself." << endl;
	std::thread hProducerThread (ProducerThread, (void*)&ctxt);
	
	// wait for threads to complete
	// wait for both threads are completed...
	// for now just wait 5 secs
	// usleep( 5*1000*1000 ); //5 secs [us]
	std::cout << "Wait for producer thread to complete\n";
	hProducerThread.join();
	std::cout << "Wait for consumer thread to complete\n";
	hConsumerThread.join();

	cout << "End of main" << endl;
	return 0;
}
