#ifndef __UTIL_SIGNAL_H__
#define __UTIL_SIGNAL_H__

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <bengine/util/mutex.h>
#endif

namespace blib
{
	namespace util
	{
		class Signal
		{
		private:
#ifdef WIN32
			HANDLE handle;
#else
			pthread_cond_t handle;
			Mutex mutex;
#endif
		public:
			Signal();
			~Signal();
			void wait();
			void signal();
		};


	}
}









#endif