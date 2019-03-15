#pragma once

#include <blib/config.h>
#include "AudioManager.h"
#include <blib/util/stb_vorbis.h>
#include <vector>
#include <thread>
#include <mutex>

#ifdef BLIB_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <al/al.h>
#include <al/alc.h>
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

		int index;

		bool isPlaying();
	};


	class OpenALAudioSample : public AudioSample
	{
	public:
		//ogg stuff
		std::string fileName;
		char* fileData;
		stb_vorbis* stream;
		stb_vorbis_info info;
		ALuint buffers[2];
		ALenum format;
		bool paused = false;
		bool buffer(ALuint buffer);
		bool updating = false;

		//wav stuff
		ALuint bufferId;


		//generic stuff
		AudioManagerOpenAL* manager;
		Source* source;

		bool looping;
		bool playing;
        int volume = 100;



		~OpenALAudioSample();
		virtual void play(bool loop) override;
		virtual void stop() override;
		virtual bool isPlaying() override;
		virtual void setVolume(int volume) override;
		virtual void pause();

        virtual bool isPaused() { return this->paused; }

		virtual bool update();

	};

	class AudioManagerOpenAL : public AudioManager
	{
	public:
		ALCdevice *device;
		std::mutex mutex;
		std::vector<Source> sources;
		std::vector<OpenALAudioSample*> samples;
		int lastSource = 0;
		bool running;
		bool alive = true;
	public:
		std::thread backgroundThread;
		AudioManagerOpenAL();
		virtual ~AudioManagerOpenAL();
		virtual void init() override;
		virtual void playMusic( std::string filename ) override;
		virtual void stopMusic() override;
		virtual void playSound( std::string filename ) override;
		virtual AudioSample* loadSample(const std::string &filename) override;

		virtual OpenALAudioSample* loadSampleWav(const std::string &filename);
		virtual OpenALAudioSample* loadSampleOgg(const std::string &filename);

		Source* getFreeSource();
		virtual void stopAllSounds() override;

		virtual void update() override; //TODO: move to background thread
		virtual void sleep() override;
		virtual void resume() override;
	};
}
