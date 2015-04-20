#pragma once

#include "AudioManager.h"
#include <al/al.h>
namespace blib
{
	class OpenALAudioSample : public AudioSample
	{
	public:
		ALuint source;

		virtual void play() override;
		virtual void stop() override;

	};

	class AudioManagerOpenAL : public AudioManager
	{
	public:
		virtual void init();
		virtual void playMusic( std::string filename );
		virtual void stopMusic();
		virtual void playSound( std::string filename );
		virtual AudioSample* loadSample(const std::string &filename);
	};
}
