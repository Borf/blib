#include "Thread.h"

#ifndef WIN32
#include <sys/prctl.h>
#include <unistd.h>
#endif

namespace blib
{
	namespace util
	{

#ifdef WIN32
		const DWORD MS_VC_EXCEPTION=0x406D1388;

#pragma pack(push,8)
		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType; // Must be 0x1000.
			LPCSTR szName; // Pointer to name (in user addr space).
			DWORD dwThreadID; // Thread ID (-1=caller thread).
			DWORD dwFlags; // Reserved for future use, must be zero.
		} THREADNAME_INFO;
#pragma pack(pop)

		void SetThreadName( DWORD dwThreadID, char* threadName)
		{
			THREADNAME_INFO info;
			info.dwType = 0x1000;
			info.szName = threadName;
			info.dwThreadID = dwThreadID;
			info.dwFlags = 0;

			__try
			{
				RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
			}
		}
#endif




		Thread::Thread(std::string pName) : name(pName)
		{
			running = false;
		}

		Thread::~Thread()
		{
#ifdef WIN32
			WaitForSingleObject(threadHandle, INFINITE);
#else
			if(running)
				pthread_join(thread, NULL);
#endif
		}

		void Thread::start()
		{
#ifdef WIN32
			threadHandle = CreateThread(NULL, 0, Thread::threadStarter, this, 0, &threadId);
#else
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

			pthread_create(&thread, &attr, threadStarter, this);
			pthread_attr_destroy(&attr);
#endif
			running = true;
		}

#ifdef WIN32
		DWORD WINAPI Thread::threadStarter( LPVOID lpParam )
		{
			SetThreadName(((Thread*)lpParam)->threadId, (char*)((Thread*)lpParam)->name.c_str());
			return ((Thread*)lpParam)->run();
		}
#else
		void* Thread::threadStarter(void* lpParam)
		{
#ifdef WIN32
#else
			prctl(PR_SET_NAME, (unsigned long)((Thread*)lpParam)->name.substr(0, std::min(16, (int)((Thread*)lpParam)->name.size())).c_str());
#endif
			pthread_exit((void*)((Thread*)lpParam)->run());
		}
#endif


		void Thread::waitForTermination()
		{
#ifdef WIN32
			WaitForSingleObject(threadHandle, INFINITE);
#else
			void * bla;
			pthread_join(thread, &bla);
#endif
		}


		void Thread::sleep(int i)
		{
#ifdef WIN32
			Sleep(i*1000);
#else
			::sleep(i);
#endif
		}


	}
}