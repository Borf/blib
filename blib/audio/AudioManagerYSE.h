#pragma once

#include <blib/config.h>
#include "AudioManager.h"
#include <blib/util/stb_vorbis.h>
#include <vector>
#include <thread>

#include <yse.hpp>


namespace blib
{
	class AudioManagerYSE;

	class YSEAudioSample : public AudioSample
	{
	public:
		//generic stuff
		AudioManagerYSE* manager;
		YSE::sound sound;
		bool looping;
		bool playing;

		YSEAudioSample(const std::string &fileName);

		virtual void play(bool loop) override;
		virtual void stop() override;
		virtual bool isPlaying() override { return true;  };
		virtual void setVolume(int volume) { };

		virtual bool update() { return true; };

	};

	class AudioManagerYSE : public AudioManager
	{
	public:
		int lastSource = 0;
		bool running;
	public:
		AudioManagerYSE();
		virtual ~AudioManagerYSE();
		virtual void init();
		virtual void playMusic(std::string filename) {};
		virtual void stopMusic() {};
		virtual void playSound(std::string filename) {};
		virtual AudioSample* loadSample(const std::string &filename);

		virtual void update(); //TODO: move to background thread
	};
}
