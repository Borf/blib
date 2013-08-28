#pragma once

#include <Windows.h>


namespace blib
{
	namespace util
	{
		class Semaphore
		{
			HANDLE ghSemaphore;
		public:
			Semaphore(int startValue, int maxValue)
			{
				ghSemaphore = CreateSemaphore( 
					NULL,           // default security attributes
					startValue,  // initial count
					maxValue,  // maximum count
					NULL);          // unnamed semaphore
			}

			void wait()
			{
				WaitForSingleObject(ghSemaphore, INFINITE);
			}

			void signal()
			{
				if (!ReleaseSemaphore( 
					ghSemaphore,  // handle to semaphore
					1,            // increase count by one
					NULL) )       // not interested in previous count
				{
					printf("ReleaseSemaphore error: %d\n", GetLastError());
				}
			}

		};
	}
}