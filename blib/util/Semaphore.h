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
					printf("ReleaseSemaphore error: %d\n", GetLastError());
				}
#else

#endif
			}

		};
	}
}