#pragma once

#include <string>
#include <functional>

namespace blib
{
	class AudioSample
	{
	public:
		virtual ~AudioSample() {};
		bool canOnlyPlayOnce = false;

		virtual void setVolume(int volume) = 0;
		virtual void play(bool loop = false) = 0;
		virtual void stop() = 0;
		virtual bool isPlaying() = 0;

		int userData = 0;
	};

	class AudioManager
	{
	public:
		std::function<bool(AudioSample*)> canPlay = nullptr;

		AudioManager() {};
		virtual ~AudioManager() {};
		static AudioManager* getInstance(); // is implemented in AudioManagerWindows.cpp and AudioManagerAndroid.cpp

		virtual void init() = 0;
		virtual void playMusic(std::string filename) = 0;
		virtual void stopMusic() = 0;

		virtual AudioSample* loadSample(const std::string &filename) = 0;

		virtual void playSound(std::string filename) = 0;


		virtual void stopAllSounds() {};

		virtual void update() {};


		virtual void sleep() {};
		virtual void resume() {};
	};
}