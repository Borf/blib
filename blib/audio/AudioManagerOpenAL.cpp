#include "AudioManagerOpenAL.h"


#include <blib/util/Log.h>
#ifdef BLIB_IOS
#include <OpenAL/alc.h>

#define SUCCEEDED(hr) (((int)(hr)) >= 0)

#else
#include <al/alc.h>
#endif
#include "CWaves.h"




using blib::util::Log;

namespace blib
{
    void checkError()
    {
        int error = alGetError();
        if(error != AL_NO_ERROR)
            Log::out << " Error: " << error << Log::newline;

    }
    
	static void list_audio_devices(const ALCchar *devices)
	{
		const ALCchar *device = devices, *next = devices + 1;
		size_t len = 0;

		fprintf(stdout, "Devices list:\n");
		fprintf(stdout, "----------\n");
		while (device && *device != '\0' && next && *next != '\0') {
			fprintf(stdout, "%s\n", device);
			len = strlen(device);
			device += (len + 1);
			next += (len + 2);
		}
		fprintf(stdout, "----------\n");
	}


	AudioManager* AudioManager::getInstance()
	{
		static AudioManager* mgr = new AudioManagerOpenAL();
		return mgr;
	}

	AudioManagerOpenAL::~AudioManagerOpenAL()
	{
//TODO		alcDestroyContext(context);
	}

	void AudioManagerOpenAL::init()
	{
		ALCdevice *device;
		device = alcOpenDevice(NULL);
		if (!device)
			Log::out << "Error initializing OpenAL" << Log::newline;
		ALboolean enumeration;
        checkError();
		enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
		if (enumeration == AL_FALSE)
			Log::out << "Enumeration not supported" << Log::newline;
		else
			Log::out << "Enumeration supported" << Log::newline;
        checkError();
		//list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
        checkError();


		ALCcontext *context;

        checkError();
		context = alcCreateContext(device, NULL);
        checkError();
		if (!alcMakeContextCurrent(context))
			Log::out << "Unable to create audio context" << Log::newline;
        checkError();

		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

		alListener3f(AL_POSITION, 0, 0, 1.0f);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		alListenerfv(AL_ORIENTATION, listenerOri);
        checkError();


		for (int i = 0; i < 32; i++)
		{
			Source source;
			source.lastSample = NULL;
			source.sourceId = 0;
			alGenSources((ALuint)1, &source.sourceId);
			alSourcef(source.sourceId, AL_PITCH, 1.0f);
			alSourcef(source.sourceId, AL_GAIN, 1.0f);
			alSource3f(source.sourceId, AL_POSITION, 0, 0, 0);
			alSource3f(source.sourceId, AL_VELOCITY, 0, 0, 0);
			alSourcei(source.sourceId, AL_LOOPING, AL_FALSE);
			sources.push_back(source);
			checkError();
		}

	}

	void AudioManagerOpenAL::playMusic(std::string filename)
	{

	}

	void AudioManagerOpenAL::stopMusic()
	{

	}

	static int i = 2;
	void AudioManagerOpenAL::playSound(std::string filename)
	{
		
	}

	AudioSample* AudioManagerOpenAL::loadSample(const std::string &filename)
	{
		ALuint buffer;
		alGenBuffers((ALuint)1, &buffer);
        checkError();

		CWaves w;

		WAVEID			WaveID;
        unsigned long			iDataSize;
        unsigned long   iFrequency;
		unsigned long			eBufferFormat;
		ALchar			*pData;
		ALboolean		bReturn;

		w.LoadWaveFile(filename.c_str(), &WaveID);

		if ((SUCCEEDED(w.GetWaveSize(WaveID, (unsigned long*)&iDataSize))) &&
			(SUCCEEDED(w.GetWaveData(WaveID, (void**)&pData))) &&
			(SUCCEEDED(w.GetWaveFrequency(WaveID, (unsigned long*)&iFrequency))) &&
			(SUCCEEDED(w.GetWaveALBufferFormat(WaveID, &alGetEnumValue, (unsigned long*)&eBufferFormat))))
		{

            checkError();
			alBufferData(buffer, eBufferFormat, pData, iDataSize, iFrequency);
            checkError();
			if (alGetError() == AL_NO_ERROR)
				bReturn = AL_TRUE;

			w.DeleteWaveFile(WaveID);

		}
        checkError();
		OpenALAudioSample* newSample = new OpenALAudioSample();
		newSample->bufferId = buffer;
		newSample->manager = this;
        checkError();        
		return newSample;
	}

	AudioManagerOpenAL::Source* AudioManagerOpenAL::getFreeSource()
	{
		int i = lastSource;
		while (((i + 1) % sources.size()) != lastSource)
		{
			if (!sources[i].isPlaying())
				return &sources[i];
			i = (i + 1) % sources.size();
		}
		return NULL;
	}


	void OpenALAudioSample::play(bool loop)
	{
		AudioManagerOpenAL::Source& source = *manager->getFreeSource();
		source.lastSample = this;
		alSourcei(source.sourceId, AL_BUFFER, 0);
		alSourcei(source.sourceId, AL_BUFFER, bufferId);
		alSourcef(source.sourceId, AL_PITCH, 1.0f);
		alSourcef(source.sourceId, AL_GAIN, 1.0f);
		alSource3f(source.sourceId, AL_POSITION, 0, 0, 0);
		alSource3f(source.sourceId, AL_VELOCITY, 0, 0, 0);
		alSourcei(source.sourceId, AL_LOOPING, loop);
		alSourcePlay(source.sourceId);
		checkError();
	}

	void OpenALAudioSample::stop()
	{
		for (AudioManagerOpenAL::Source& source : manager->sources)
		{
			if (source.lastSample == this)
				alSourceStop(source.sourceId);
		}

	}



	bool AudioManagerOpenAL::Source::isPlaying()
	{
		int sourceState;
		alGetSourcei(sourceId, AL_SOURCE_STATE, &sourceState);
		return sourceState == AL_PLAYING;
	}

}