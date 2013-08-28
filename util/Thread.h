#pragma once

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <string>

namespace blib
{
	namespace util
	{
		class Thread
		{
		private:
#ifdef WIN32
			HANDLE	threadHandle;
			static DWORD WINAPI threadStarter( LPVOID lpParam );
#else
			pthread_t thread;
			static void* threadStarter(void* param);
#endif
		protected:
			bool running;
		public:
			Thread(std::string);
			virtual ~Thread();
			void start();
			void stop();
			void kill();
			void signal();
			int getId();
			void waitForTermination();
			static  void sleep(int);
			virtual int run() = 0;

			std::string name;
#ifdef WIN32
			unsigned long threadId;
#endif
		};


	}
}