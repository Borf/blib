#pragma once

#include <chrono>


namespace blib
{
	namespace util
	{
		class Timer
		{
		private:
			std::chrono::time_point<std::chrono::system_clock> start;
			std::chrono::time_point<std::chrono::system_clock> pauseTime;
			std::chrono::milliseconds pauseLength;
			bool paused = false;
		public:
			Timer();

			void setPause(bool paused);
			inline void pause() { setPause(true); }
			inline void resume() { setPause(false); }
			inline bool isPaused() { return paused; }

			void setMillis(long milliseconds);

			long millis();
		};
	}
}