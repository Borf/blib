#include "Profiler.h"
#include "Log.h"
#include <blib/config.h>

#define SHOWFPS
#define NOPROFILE


#ifdef BLIB_IOS

#include <sys/time.h>

//clock_gettime is not implemented on OSX
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif

int clock_gettime(int clk_id, struct timespec* t)
{
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if(rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}

#endif


namespace blib
{
	namespace util
	{
#ifdef WIN32
#include <Windows.h>
		LARGE_INTEGER getFILETIMEoffset()
		{
			SYSTEMTIME s;
			FILETIME f;
			LARGE_INTEGER t;

			s.wYear = 1970;
			s.wMonth = 1;
			s.wDay = 1;
			s.wHour = 0;
			s.wMinute = 0;
			s.wSecond = 0;
			s.wMilliseconds = 0;
			SystemTimeToFileTime(&s, &f);
			t.QuadPart = f.dwHighDateTime;
			t.QuadPart <<= 32;
			t.QuadPart |= f.dwLowDateTime;
			return (t);
		}

		int	clock_gettime(int X, struct timespec *tv)
		{
			LARGE_INTEGER           t;
			FILETIME            f;
			double                  microseconds;
			static LARGE_INTEGER    offset;
			static double           frequencyToMicroseconds;
			static int              initialized = 0;
			static BOOL             usePerformanceCounter = 0;

			if (!initialized) {
				LARGE_INTEGER performanceFrequency;
				initialized = 1;
				usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
				if (usePerformanceCounter) {
					QueryPerformanceCounter(&offset);
					frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
				} else {
					offset = getFILETIMEoffset();
					frequencyToMicroseconds = 10.;
				}
			}
			if (usePerformanceCounter) QueryPerformanceCounter(&t);
			else {
				GetSystemTimeAsFileTime(&f);
				t.QuadPart = f.dwHighDateTime;
				t.QuadPart <<= 32;
				t.QuadPart |= f.dwLowDateTime;
			}

			t.QuadPart -= offset.QuadPart;
			microseconds = (double)t.QuadPart / frequencyToMicroseconds;
			t.QuadPart = (LONGLONG)microseconds;
			tv->tv_sec = t.QuadPart / 1000000;
			tv->tv_nsec = (t.QuadPart % 1000000) * 1000;
			return (0);
		}
#endif


		double Profiler::getTime()
		{
			struct timespec current;
			clock_gettime(CLOCK_REALTIME, &current);

			double dSeconds = (double)(current.tv_sec - frameBegin.tv_sec);
			double dNanoSeconds = (double)( current.tv_nsec - frameBegin.tv_nsec ) / 1000000000L;
			return dSeconds + dNanoSeconds;
		}


		double Profiler::getAppTime()
		{
			
            struct timespec current;
			clock_gettime(CLOCK_REALTIME, &current);

			double dSeconds = (double)(current.tv_sec - appBegin.tv_sec);
			double dNanoSeconds = (double)( current.tv_nsec - appBegin.tv_nsec ) / 1000000000L;
			return dSeconds + dNanoSeconds;
		}


		timespec Profiler::frameBegin;
		timespec Profiler::appBegin;
		int Profiler::frameCount;
		timespec Profiler::fpscalc;
		int Profiler::fpsFrameCount;
		int Profiler::fps;

		std::list<std::map<std::string, double> > Profiler::frameTimes;
		std::map<std::string, double> Profiler::currentFrameTimes;


		Profiler::Profiler()
		{
			frameCount = 0;
			fpsFrameCount = 0;
			fps = 0;
			clock_gettime(CLOCK_REALTIME, &frameBegin);
			clock_gettime(CLOCK_REALTIME, &appBegin);
		}


		void Profiler::startFrame()
		{
			frameCount++;
#ifdef SHOWFPS
			fpsFrameCount++;
			if(frameBegin.tv_sec - fpscalc.tv_sec >= 1)
			{
				fps = (int)(fpsFrameCount/(frameBegin.tv_sec - fpscalc.tv_sec) + (double)(frameBegin.tv_nsec - fpscalc.tv_nsec) / 1000000000L);
				fpscalc = frameBegin;
				fpsFrameCount = 0;
			}
#endif
#ifndef NOPROFILE
			frameTimes.push_back(currentFrameTimes);
			if(frameCount > 1000)
			{
				std::map<std::string, double> totals;
				std::map<std::string, double> maxValues;
				for(std::map<std::string, double>::iterator it = currentFrameTimes.begin(); it != currentFrameTimes.end(); it++)
				{
					totals[it->first] = 0;
					maxValues[it->first] = 0;
				}

				for(std::list<std::map<std::string, double> >::iterator it = frameTimes.begin(); it != frameTimes.end(); it++)
				{
					for(std::map<std::string, double>::iterator it2 = it->begin(); it2 != it->end(); it2++)
					{
						totals[it2->first] += it2->second;
						if(maxValues[it2->first] < it2->second)
							maxValues[it2->first] = it2->second;
					}
				}

				Log::out<<"------------------------------"<<Log::newline;
				for(std::map<std::string, double>::iterator it = totals.begin(); it != totals.end(); it++)
				{
					Log::out<<it->first<<" -> "<<(it->second / frameTimes.size())*1000<<"ms, max "<<maxValues[it->first]*1000<<"ms"<<Log::newline;
				}

				frameCount = 0;
				frameTimes.clear();
			}
			currentFrameTimes.clear();
#endif
			clock_gettime(CLOCK_REALTIME, &frameBegin);
		}


		void Profiler::startSection(std::string name)
		{
#ifndef NOPROFILE
			currentFrameTimes[name] = getTime();
#endif
		}


		void Profiler::endSection(std::string name)
		{
#ifndef NOPROFILE
			currentFrameTimes[name] = getTime() - currentFrameTimes[name];
#endif
		}


	}
}
