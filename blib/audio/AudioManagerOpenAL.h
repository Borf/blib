#pragma once

#include <blib/config.h>
#include "AudioManager.h"
#include <blib/util/stb_vorbis.h>
#include <vector>

#ifdef BLIB_IOS
#include <OpenAL/al.h>
#else
#include <al/al.h>
#endif

namespace blib
{
	class AudioManagerOpenAL;
	class OpenALAudioSample;

	class Source
	{
	public:
		OpenALAudioSample* lastSample = NULL;
		ALuint sourceId;

		bool isPlaying();
	};


	class OpenALAudioSample : public AudioSample
	{
	public:
		//ogg stuff
		char* fileData;
		stb_vorbis* stream;
		stb_vorbis_info info;
		ALuint buffers[2];
		ALenum format;
		size_t totalSamplesLeft;
		bool buffer(ALuint buffer);

		//wav stuff
		ALuint bufferId;


		//generic stuff
		AudioManagerOpenAL* manager;
		Source* source;

		bool looping;
		bool playing;

		virtual void play(bool loop) override;
		virtual void stop() override;

		virtual bool update();

	};

	class AudioManagerOpenAL : public AudioManager
	{
	public:

		std::vector<Source> sources;
		std::vector<OpenALAudioSample*> samples;
		int lastSource = 0;

	public:

		virtual ~AudioManagerOpenAL();
		virtual void init();
		virtual void playMusic( std::string filename );
		virtual void stopMusic();
		virtual void playSound( std::string filename );
		virtual AudioSample* loadSample(const std::string &filename);

		virtual OpenALAudioSample* loadSampleWav(const std::string &filename);
		virtual OpenALAudioSample* loadSampleOgg(const std::string &filename);

		Source* getFreeSource();

		virtual void update(); //TODO: move to background thread
	};
}
