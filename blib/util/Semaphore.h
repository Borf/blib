#pragma once

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif


namespace blib
{
	namespace util
	{
		class Semaphore
		{
#ifdef WIN32
			HANDLE ghSemaphore;
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

#endif
			}

			void wait()
			{
#ifdef WIN32
				WaitForSingleObject(ghSemaphore, INFINITE);
#else

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

#endif
			}

		};
	}
}