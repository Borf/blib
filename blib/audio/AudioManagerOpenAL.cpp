#include "AudioManagerOpenAL.h"


#include <blib/util/Log.h>
#include <al/al.h>
#include <al/alc.h>
#include "CWaves.h"
#include <direct.h>

using blib::util::Log;

namespace blib
{
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

	void AudioManagerOpenAL::init()
	{
		ALCdevice *device;
		device = alcOpenDevice(NULL);
		if (!device)
			Log::out << "Error initializing OpenAL" << Log::newline;
		ALboolean enumeration;

		enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
		if (enumeration == AL_FALSE)
			Log::out << "Enumeration not supported" << Log::newline;
		else
			Log::out << "Enumeration supported" << Log::newline;
		list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));


		ALCcontext *context;

		context = alcCreateContext(device, NULL);
		if (!alcMakeContextCurrent(context))
			Log::out << "Unable to create audio context" << Log::newline;

		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

		alListener3f(AL_POSITION, 0, 0, 1.0f);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		alListenerfv(AL_ORIENTATION, listenerOri);

	}

	void AudioManagerOpenAL::playMusic(std::string filename)
	{
/*		mciSendString("close song1", NULL, 0, 0);
		mciSendString(("open "+filename+" type mpegvideo alias song1").c_str(), NULL, 0, 0); 
		mciSendString("play song1 repeat", NULL, 0, 0);*/
	}

	void AudioManagerOpenAL::stopMusic()
	{
	//	mciSendString("stop song1", NULL, 0, 0);
	}

	static int i = 2;
	void AudioManagerOpenAL::playSound(std::string filename)
	{
		/*char buf[1024];
		int r = i;
		i++;
		if(i > 10000)
			i = 2;
		sprintf(buf, "open %s type mpegvideo alias audio%i", filename.c_str(), r);
		int ret = mciSendString(buf, NULL, 0, 0); 

		if(ret != 0)
		{
			mciGetErrorString(ret, buf, 1024);
			printf("%s\n", buf);
		}

		sprintf(buf, "play audio%i", r);
		ret = mciSendString(buf, NULL, 0, 0);

		if(ret != 0)
		{
			mciGetErrorString(ret, buf, 1024);
			printf("%s\n", buf);
		}*/
	}

	AudioSample* AudioManagerOpenAL::loadSample(const std::string &filename)
	{
		ALuint source;
		alGenSources((ALuint)1, &source);
		alSourcef(source, AL_PITCH, 1);
		alSourcef(source, AL_GAIN, 1);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		alSourcei(source, AL_LOOPING, AL_FALSE);


		ALuint buffer;
		alGenBuffers((ALuint)1, &buffer);

		CWaves w;

		WAVEID			WaveID;
		ALint			iDataSize, iFrequency;
		ALenum			eBufferFormat;
		ALchar			*pData;
		ALboolean		bReturn;

		w.LoadWaveFile(filename.c_str(), &WaveID);

		if ((SUCCEEDED(w.GetWaveSize(WaveID, (unsigned long*)&iDataSize))) &&
			(SUCCEEDED(w.GetWaveData(WaveID, (void**)&pData))) &&
			(SUCCEEDED(w.GetWaveFrequency(WaveID, (unsigned long*)&iFrequency))) &&
			(SUCCEEDED(w.GetWaveALBufferFormat(WaveID, &alGetEnumValue, (unsigned long*)&eBufferFormat))))
		{

			alBufferData(buffer, eBufferFormat, pData, iDataSize, iFrequency);
			if (alGetError() == AL_NO_ERROR)
				bReturn = AL_TRUE;

			w.DeleteWaveFile(WaveID);

		}

		//assign the buffer to this source
		alSourcei(source, AL_BUFFER, buffer);

		OpenALAudioSample* newSample = new OpenALAudioSample();
		newSample->source = source;
		return newSample;
	}


	void OpenALAudioSample::play()
	{
		alSourcePlay(source);
	}

	void OpenALAudioSample::stop()
	{
	}

}