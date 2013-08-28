#include "signal.h"


namespace blib
{
	namespace util
	{
		Signal::Signal()
		{
#ifdef WIN32
			handle = CreateEvent(NULL, TRUE, FALSE, "Event");
#else
			pthread_cond_init(&handle, NULL);
#endif
		}

		Signal::~Signal()
		{
#ifdef WIN32
			CloseHandle(handle);
#else
			pthread_cond_destroy(&handle);
#endif
		}

		void Signal::wait()
		{
#ifdef WIN32
			ResetEvent(handle);
			WaitForSingleObject(handle, INFINITE);
#else
			mutex.lock();
			pthread_cond_wait(&handle, &mutex.mutex);
			mutex.unLock();
#endif
		}

		void Signal::signal()
		{
#ifdef WIN32
			SetEvent(handle);
#else
			pthread_cond_signal(&handle);
#endif
		}
	}
}