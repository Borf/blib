#pragma once

#include <time.h>
#include <string>
#include <list>
#include <map>

#include <blib/config.h>

#ifdef BLIB_VISUALSTUDIO
#define CLOCK_REALTIME 0
/*struct timespec
{
	time_t	tv_sec;
	long	tv_nsec;
};*/
#endif

namespace blib
{
	namespace util
	{
		class Profiler
		{
			static timespec appBegin;
			static timespec frameBegin;
			static timespec fpscalc;
			static int fpsFrameCount;
			static int frameCount;

			static std::list<std::map<std::string, double> > frameTimes;
			static std::map<std::string, double> currentFrameTimes;
	
		public:
			Profiler();

			static double getTime();
			static double getAppTime();
	
			static void startFrame();

			static void startSection(std::string name);
			static void endSection(std::string name);

			static int fps;
		};

	}
}
