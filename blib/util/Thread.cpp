#include "Thread.h"

#ifdef WIN32
#include <blib/linq.h>
#endif

#include <blib/config.h>

#if defined(BLIB_ANDROID) || defined(BLIB_LINUX)
#include <sys/prctl.h>
#endif

#if defined(BLIB_IOS) || defined(BLIB_LINUX) || defined(BLIB_ANDROID) || defined(BLIB_EMSCRIPTEN) || defined(BLIB_NX)
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

#ifdef BLIB_WIN
		std::map<int, std::string> Thread::threadNames;
#endif
		void SetThreadName( DWORD dwThreadID, const char* threadName)
		{
			Thread::threadNames[dwThreadID] = threadName;
#ifdef _MSC_VER
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
#endif
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
			int threadId = ((Thread*)lpParam)->threadId;
			std::string name = ((Thread*)lpParam)->name;
			SetThreadName(threadId, (char*)name.c_str());

			if(linq::contains(threadNames, [name] (std::pair<int, std::string> p) { return p.second == name; }))
			{
				static std::map<std::string, int> nameIndices;
				nameIndices[name]++;
				char newName[100];
				sprintf(newName, "%s%03i", name.c_str(), ++nameIndices[name]);
				name = newName;
			}
			threadNames[threadId] = name;
			return ((Thread*)lpParam)->run();
		}
#else
		void* Thread::threadStarter(void* lpParam)
		{
#ifdef WIN32
#elif defined(BLIB_ANDROID) || defined(BLIB_LINUX)
            prctl(
                        PR_SET_NAME,
                        (unsigned long)((Thread*)lpParam)->name.substr(
                            0,
                            std::min(
                                16,
                                (int)((Thread*)lpParam)->name.size())
                          ).c_str()
                  );
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

		std::string Thread::getCurrentThreadName()
		{
#ifdef WIN32
			int id = GetCurrentThreadId();
			if (threadNames.find(id) != threadNames.end())
				return threadNames[id];
			return "error";
#else
			return "";
#endif
		}

		void Thread::setMainThread()
		{
#ifdef WIN32
			threadNames[GetCurrentThreadId()] = "MainThread";
#endif
		}

		void Thread::setThreadName(const std::string &name)
		{
		#ifdef BLIB_WIN
			SetThreadName(GetCurrentThreadId(), name.c_str());
		#endif
		}


	}
}
