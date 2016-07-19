#include "AudioManagerOpenAL.h"

#include <blib/util/Thread.h>
#include <blib/util/Log.h>
#ifdef BLIB_IOS
#include <OpenAL/alc.h>

#define SUCCEEDED(hr) (((int)(hr)) >= 0)
#include <unistd.h>
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

	AudioManagerOpenAL::AudioManagerOpenAL()
	{
		running = true;
		backgroundThread = std::thread([this]()
		{
			blib::util::Thread::setThreadName("AudioManager");
			while (running)
			{
				update();
#ifdef BLIB_WIN
				Sleep(10);
#else
				usleep(10000);
#endif
			}
		});
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
			source.index = i;
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

    void AudioManagerOpenAL::playSound(std::string filename)
	{
		
	}

	AudioSample* AudioManagerOpenAL::loadSample(const std::string &filename)
	{
		OpenALAudioSample* sample = NULL;
		if (filename.substr(filename.size() - 4) == ".wav")
			sample = loadSampleWav(filename);
		else if (filename.substr(filename.size() - 4) == ".ogg")
			sample = loadSampleOgg(filename);
		if (sample != NULL)
		{
			samples.push_back(sample);
		}
		return sample;
	}


	OpenALAudioSample* AudioManagerOpenAL::loadSampleWav(const std::string &filename)
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
		newSample->fileName = filename;
		newSample->playing = false;
		newSample->bufferId = buffer;
		newSample->manager = this;
		checkError();
		return newSample;
	}
	

	OpenALAudioSample* AudioManagerOpenAL::loadSampleOgg(const std::string &filename)
	{

		char* data;
		int len = blib::util::FileSystem::getData(filename, data);

		stb_vorbis* stream = stb_vorbis_open_memory(const_cast<const unsigned char*>((unsigned char*)data), len, NULL, NULL); 
		if (!stream)
		{
			Log::out << "AudioManagerOpenAL: Could not open " << filename << Log::newline;
			return NULL;
		}
		stb_vorbis_info info = stb_vorbis_get_info(stream);


		checkError();
		OpenALAudioSample* newSample = new OpenALAudioSample();
		newSample->fileName = filename;
		newSample->bufferId = 0;
		alGenBuffers((ALuint)2, newSample->buffers);
		newSample->fileData = data;
		newSample->manager = this;
		newSample->info = info;
		newSample->stream = stream;
		newSample->playing = false;
		if (info.channels == 2)
			newSample->format = AL_FORMAT_STEREO16;
		else
			newSample->format = AL_FORMAT_MONO16;

		checkError();

		newSample->buffer(newSample->buffers[0]);
		newSample->buffer(newSample->buffers[1]);

		return newSample;
	}

	bool OpenALAudioSample::buffer(ALuint buffer)
	{
		Log::out << "AudioManager: buffering sound " << fileName << " to buffer " << buffer << Log::newline;
		//Uncomment this to avoid VLAs
		//#define BUFFER_SIZE 4096*32
#ifndef BUFFER_SIZE//VLAs ftw
#define BUFFER_SIZE 4096*32*4
#endif
		ALshort* pcm = new ALshort[BUFFER_SIZE];
		int  size = 0;
		int  result = 0;

		while (size < BUFFER_SIZE) {
			result = stb_vorbis_get_samples_short_interleaved(stream, info.channels, pcm + size, BUFFER_SIZE - size);
			if (result > 0) size += result*info.channels;
			else break;
		}

		if (size == 0)
		{
			Log::out<<"AudioManager: no more bytes to read from ogg"<<Log::newline;
			delete[] pcm;
			return false;
		}

		alBufferData(buffer, format, pcm, size*sizeof(ALshort), info.sample_rate);
#undef BUFFER_SIZE
		delete[] pcm;
		return true;
	}

	Source* AudioManagerOpenAL::getFreeSource()
	{
		int i = lastSource;
		while (((i + 1) % sources.size()) != lastSource)
		{
			if (!sources[i].isPlaying())
			{
				if(sources[i].lastSample)
					sources[i].lastSample->source = nullptr;
				return &sources[i];
			}
			i = (i + 1) % sources.size();
		}
		return NULL;
	}






	void OpenALAudioSample::play(bool loop)
	{
		if (canOnlyPlayOnce && playing && isPlaying())
			return;
		if (manager->canPlay && !manager->canPlay(this))
			return;

		manager->mutex.lock();
		Source* newSource = manager->getFreeSource();
		if (!newSource)
		{
			manager->mutex.unlock();
			return;
		}
		source = newSource;
		source->lastSample = this;
		this->looping = loop;

		Log::out << "Playing " << this->fileName << " on source " << source->index << Log::newline;
		
		if (bufferId != 0) //wav
		{
			alSourcei(source->sourceId, AL_BUFFER, 0);
			alSourcei(source->sourceId, AL_BUFFER, bufferId);
			alSourcef(source->sourceId, AL_PITCH, 1.0f);
			alSourcef(source->sourceId, AL_GAIN, 1.0f);
			alSource3f(source->sourceId, AL_POSITION, 0, 0, 0);
			alSource3f(source->sourceId, AL_VELOCITY, 0, 0, 0);
			alSourcei(source->sourceId, AL_LOOPING, loop);
			alSourcePlay(source->sourceId);
		}
		else //ogg
		{
			alSourcef(source->sourceId, AL_PITCH, 1.0f);
			alSourcef(source->sourceId, AL_GAIN, 1.0f);
			alSource3f(source->sourceId, AL_POSITION, 0, 0, 0);
			alSource3f(source->sourceId, AL_VELOCITY, 0, 0, 0);
			alSourcei(source->sourceId, AL_LOOPING, 0);
			alSourcei(source->sourceId, AL_BUFFER, 0);

			checkError();
			alSourceQueueBuffers(source->sourceId, 2, buffers);
			checkError();
			alSourcePlay(source->sourceId);
			checkError();
		}
		playing = true;
		checkError();
		manager->mutex.unlock();
	}

	void OpenALAudioSample::stop()
	{
		manager->mutex.lock();
		if (!playing || !isPlaying())
		{
			if (source)
			{
				alSourceStop(source->sourceId);
				source->lastSample = nullptr;
			}
			playing = false;
			source = nullptr;
			manager->mutex.unlock();
			return;
		}
		playing = false;
		if (source)
		{
			alSourceStop(source->sourceId);
			source->lastSample = nullptr;
		}
		if(bufferId == 0 && source)
			alSourceUnqueueBuffers(source->sourceId, 2, buffers);
		source = nullptr;
		manager->mutex.unlock();
	}

	bool OpenALAudioSample::isPlaying()
	{
		if (!source)
			return false;
		return source->isPlaying();
	}

	void OpenALAudioSample::setVolume(int volume)
	{
		if(source)
			alSourcef(source->sourceId, AL_GAIN, volume/100.0f);
	}

	bool OpenALAudioSample::update()
	{
		if (bufferId == 0 && source && playing) //only for playing audio files
		{
			ALint processed = 0;
			alGetSourcei(source->sourceId, AL_BUFFERS_PROCESSED, &processed);

			while (processed--) {
				ALuint buf = 0;

				alSourceUnqueueBuffers(source->sourceId, 1, &buf);
				if(buf != 0)
				{
					if (!buffer(buf)) {
						bool shouldExit = true;

						if (looping) {
							Log::out<<"AudioManager: restarting sound"<<Log::newline;
							stb_vorbis_seek_start(stream);
							shouldExit = !buffer(buf);
							if(shouldExit)
								Log::out<<"AudioManager: no sound after restarting vorbis stream"<<Log::newline;
							alSourcePlay(source->sourceId);
						}

						if (shouldExit) {
							Log::out<<"Stopping "<<fileName<<Log::newline;
							playing = false;
							return false;
						}
					}
					alSourceQueueBuffers(source->sourceId, 1, &buf);
				}
			}

		}
		return true;
	}


	bool Source::isPlaying()
	{
		int sourceState;
		alGetSourcei(sourceId, AL_SOURCE_STATE, &sourceState);
		return sourceState == AL_PLAYING;
	}

	void AudioManagerOpenAL::update()
	{
		mutex.lock();
		for (OpenALAudioSample* sample : samples)
			sample->update();
		mutex.unlock();
	}

}