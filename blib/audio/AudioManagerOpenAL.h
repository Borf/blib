#pragma once

#include <blib/config.h>
#include "AudioManager.h"

#ifdef BLIB_IOS
#include <OpenAL/al.h>
#else
#include <al/al.h>
#endif

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
