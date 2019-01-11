#include "Timer.h"

#include <chrono>

namespace blib
{
	namespace util
	{
		Timer::Timer()
		{
			start = std::chrono::system_clock::now();
		}


		void Timer::setPause(bool paused)
		{
			if (this->paused == paused)
				return;

			if (paused)
				pauseLength += std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now() - pauseTime
					);
			else
				pauseTime = std::chrono::system_clock::now();
			this->paused = paused;
		}

		long Timer::millis()
		{
			if (paused)
				return (long)(std::chrono::duration_cast<std::chrono::milliseconds>(
					pauseTime - start
					).count() - pauseLength.count());
			else
				return (long)(std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now() - start
					).count() - pauseLength.count());
		}
		
		void Timer::setMillis(long milliseconds)
		{
			pauseLength = std::chrono::milliseconds(0);
			pauseTime = start = std::chrono::system_clock::now() - std::chrono::milliseconds(milliseconds);
			if (paused)
				pauseTime = std::chrono::system_clock::now();
		}
	}
}