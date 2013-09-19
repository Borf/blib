#include <blib/util/Mutex.h>
#include <blib/util/Log.h>

namespace blib
{
	namespace util
	{
		void Mutex::lock()
		{
#ifdef WIN32
			EnterCriticalSection(&cs);
			if(debugEnabled)
				Log::out<<"Mutex "<<(int)&cs<<" Lock";
#else
			pthread_mutex_lock(&mutex);
#endif
		}

		void Mutex::unLock()
		{
#ifdef WIN32
			LeaveCriticalSection(&cs);
			if(debugEnabled)
				Log::out<<"Mutex "<<(int)&cs<<" unLock";
#else
			pthread_mutex_unlock(&mutex);
#endif
		}

		Mutex::Mutex()
		{
#ifdef WIN32
			InitializeCriticalSection(&cs);
#else
			pthread_mutex_init(&mutex, NULL);
#endif
			debugEnabled = false;
		}

		Mutex::~Mutex()
		{
#ifdef WIN32
			DeleteCriticalSection(&cs);
#else
			pthread_mutex_destroy(&mutex);
#endif
		}
	}
}
