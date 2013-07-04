#pragma once

#include <time.h>
#include <string>
#include <list>
#include <map>

#ifdef WIN32
#define CLOCK_REALTIME 0
struct timespec
{
	time_t	tv_sec;
	long	tv_nsec;
};
#endif

namespace blib
{
	namespace util
	{
		class Profiler
		{
			static timespec frameBegin;
			static timespec fpscalc;
			static int fpsFrameCount;
			static int frameCount;
			static double getTime();

			static std::list<std::map<std::string, double> > frameTimes;
			static std::map<std::string, double> currentFrameTimes;
	
		public:
			Profiler();

	
			static void startFrame();

			static void startSection(std::string name);
			static void endSection(std::string name);
		};

	}
}
