#pragma once

#include <string>

namespace blib
{
	class AudioSample
	{
	public:
		virtual void play() = 0;
		virtual void stop() = 0;
	};

	class AudioManager
	{
	public:
		AudioManager() {};
		virtual ~AudioManager() {};
		static AudioManager* getInstance(); // is implemented in AudioManagerWindows.cpp and AudioManagerAndroid.cpp

		virtual void init() = 0;
		virtual void playMusic(std::string filename) = 0;
		virtual void stopMusic() = 0;

		virtual AudioSample* loadSample(const std::string &filename) = 0;

		virtual void playSound(std::string filename) = 0;
	};
}