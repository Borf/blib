#ifndef __UTIL_MUTEX_H__
#define __UTIL_MUTEX_H__

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
class Signal;
#endif


namespace blib
{
	namespace util
	{
		class Mutex
		{
		private:
#ifdef WIN32
			CRITICAL_SECTION cs;
#else
			pthread_mutex_t	mutex;
#endif
		public:
			bool debugEnabled;
			Mutex();
			~Mutex();

			void lock();
			void unLock() ;
			friend class Signal;			
		};
	}
}


#endif