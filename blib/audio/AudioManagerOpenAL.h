#pragma once

#include <blib/config.h>
#include "AudioManager.h"

#include <vector>

#ifdef BLIB_IOS
#include <OpenAL/al.h>
#else
#include <al/al.h>
#endif

namespace blib
{
	class AudioManagerOpenAL;

	class OpenALAudioSample : public AudioSample
	{
	public:
		ALuint bufferId;
		AudioManagerOpenAL* manager;

		virtual void play(bool loop) override;
		virtual void stop() override;

	};

	class AudioManagerOpenAL : public AudioManager
	{
	public:
		class Source
		{
		public:
			OpenALAudioSample* lastSample = NULL;
			ALuint sourceId;

			bool isPlaying();
		};

		std::vector<Source> sources;
		int lastSource = 0;

	public:
		virtual void init();
		virtual void playMusic( std::string filename );
		virtual void stopMusic();
		virtual void playSound( std::string filename );
		virtual AudioSample* loadSample(const std::string &filename);

		Source* getFreeSource();
	};
}
