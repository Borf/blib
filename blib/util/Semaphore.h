#pragma once

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#endif


namespace blib
{
	namespace util
	{
		class Semaphore
		{
#ifdef WIN32
			HANDLE ghSemaphore;
#else
			sem_t* semaphore;
#endif
		public:
			Semaphore(int startValue, int maxValue)
			{
#ifdef WIN32
				ghSemaphore = CreateSemaphore( 
					NULL,           // default security attributes
					startValue,  // initial count
					maxValue,  // maximum count
					NULL);          // unnamed semaphore

				int errorId = GetLastError();
				if(errorId != 0)
				{
					char* lpMsgBuf;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS, NULL,	errorId,	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
					Log::out<<"ReleaseSemaphore Error: "<<lpMsgBuf<<Log::newline;
				}

#else
				char name[100];
				sprintf(name, "blib_sem_%i", rand());
				if((semaphore = sem_open(name, O_CREAT, 0644, startValue)) == SEM_FAILED)
				{
					Log::out<<"Error creating semaphore"<<Log::newline;
				}
#endif
			}

			void wait()
			{
#ifdef WIN32
				WaitForSingleObject(ghSemaphore, INFINITE);
#else
				Log::out<<"Waiting semaphore "<<Log::newline;
				sem_wait(semaphore);
#endif
			}

			void signal()
			{
#ifdef WIN32
				if (!ReleaseSemaphore( 
					ghSemaphore,  // handle to semaphore
					1,            // increase count by one
					NULL) )       // not interested in previous count
				{
					char* lpMsgBuf;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS, NULL,	GetLastError(),	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
					Log::out<<"ReleaseSemaphore Error: "<<lpMsgBuf<<Log::newline;
				}
#else
				Log::out<<"Signalling semaphore "<<Log::newline;
				sem_post(semaphore);
#endif
			}

		};
	}
}