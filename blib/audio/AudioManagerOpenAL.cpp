#include "AudioManagerOpenAL.h"

#include <blib/util/Thread.h>
#include <blib/util/Log.h>

#ifdef BLIB_IOS
#include <OpenAL/alc.h>
#endif

#ifdef BLIB_ANDROID
#include <al/alc.h>
#endif

#if defined(BLIB_IOS) || defined(BLIB_ANDROID) || defined(BLIB_NX)
#define SUCCEEDED(hr) (((int)(hr)) >= 0)
#include <unistd.h>
#else
#include <al/alc.h>
#endif
#include "CWaves.h"
#include <algorithm>


#ifdef BLIB_ANDROID


#endif

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
#ifdef _DEBUG
		Log::out << "Loading audio sample " << filename << Log::newline;
#endif
		OpenALAudioSample* sample = NULL;
		if (filename.substr(filename.size() - 4) == ".wav")
			sample = loadSampleWav(filename);
		else if (filename.substr(filename.size() - 4) == ".ogg")
			sample = loadSampleOgg(filename);
		if (sample != NULL)
		{
			std::lock_guard<std::mutex> guard(mutex);
			samples.push_back(sample);
		}
		return sample;
	}

	void AudioManagerOpenAL::unloadSample(AudioSample* sample)
	{
		toDelete.push_back(sample);
	}


	OpenALAudioSample* AudioManagerOpenAL::loadSampleWav(const std::string &filename)
	{
		ALuint buffer;
		alGenBuffers((ALuint)1, &buffer);
		checkError();


		if (blib::util::FileSystem::exists(filename + ".ogg"))
		{
			char* tmpData;
			int len = blib::util::FileSystem::getData(filename + ".ogg", tmpData);

			int chan = 2, samplerate;
			short *output;
			int samples = stb_vorbis_decode_memory((unsigned char*)tmpData, len, &chan, &samplerate, &output);
			alBufferData(buffer, chan == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, output, samples * chan * 2, samplerate);
			checkError();
			delete[] tmpData;
			delete[] output;
		}
		else
		{
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
			else
				Log::out << "Error loading wave file: " << filename << Log::newline;
			checkError();
		}
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
	//	Log::out << "AudioManager: buffering sound " << fileName << " to buffer " << buffer << Log::newline;
		//Uncomment this to avoid VLAs
		//#define BUFFER_SIZE 4096*32
#ifndef BUFFER_SIZE//VLAs ftw
#define BUFFER_SIZE 4096*64*2
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






	OpenALAudioSample::~OpenALAudioSample()
	{
		while (updating)
		{
#ifdef BLIB_WIN
			Sleep(100);
#else
			sleep(1);
#endif
		}
		if (bufferId)
		{
			assert(alIsBuffer(bufferId) == AL_TRUE);
			checkError();
			if (source)
				alSourceUnqueueBuffers(source->sourceId, 1, &bufferId);
			checkError();
			alDeleteBuffers(1, &bufferId);
			checkError();
		}

		if (manager)
		{
			manager->samples.erase(std::remove(manager->samples.begin(), manager->samples.end(), this), manager->samples.end());
		}
		if(fileData)
			delete[] fileData;
	}

	void OpenALAudioSample::play(bool loop)
	{
		if (!manager->device)
			return;
		if (canOnlyPlayOnce && playing && isPlaying())
			return;
		if (manager->canPlay && !manager->canPlay(this))
			return;

		{
			std::lock_guard<std::mutex> guard(manager->mutex);
			Source* newSource = manager->getFreeSource();
			if (!newSource)
			{
				Log::out << " Error getting free resource while playing " << this->fileName << Log::newline;
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
				alSourcef(source->sourceId, AL_GAIN, volume / 100.0f);
				alSource3f(source->sourceId, AL_POSITION, 0, 0, 0);
				alSource3f(source->sourceId, AL_VELOCITY, 0, 0, 0);
				alSourcei(source->sourceId, AL_LOOPING, loop);
				alSourcePlay(source->sourceId);
			}
			else //ogg
			{
				alSourcef(source->sourceId, AL_PITCH, 1.0f);
				alSourcef(source->sourceId, AL_GAIN, volume / 100.0f);
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
		}
	}

	void OpenALAudioSample::stop()
	{
		std::lock_guard<std::mutex> guard(manager->mutex);
		if (!playing || !isPlaying())
		{
			if (source)
			{
				alSourceStop(source->sourceId);
				source->lastSample = nullptr;
			}
			if (bufferId == 0 && source)
				alSourceUnqueueBuffers(source->sourceId, 2, buffers);
			else if(source)
				alSourceUnqueueBuffers(source->sourceId, 1, &bufferId);

			playing = false;
			source = nullptr;
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
	}

	bool OpenALAudioSample::isPlaying()
	{
		if (!source)
			return false;
		return source->isPlaying();
	}

	void OpenALAudioSample::setVolume(int volume)
	{
		if (volume < 1)
			volume = 1;
		this->volume = volume;
		if(source)
			alSourcef(source->sourceId, AL_GAIN, volume/100.0f);
	}

	void OpenALAudioSample::dispose()
	{
		manager->unloadSample(this);
	}

	bool OpenALAudioSample::update()
	{
		updating = true;
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
							updating = false;
							return false;
						}
					}
					if(source && playing) {
						alSourceQueueBuffers(source->sourceId, 1, &buf);
						if (!isPlaying())
							alSourcePlay(source->sourceId);
					}
				}
			}

		}
		updating = false;

		return true;
	}

	void OpenALAudioSample::pause() {
		if(source) {
		    if(!paused)
                alSourcePause(source->sourceId);
            else
                alSourcePlay(source->sourceId);


			paused = !paused;
		}
	}

	void AudioManagerOpenAL::sleep() {
		alive = false;

		std::lock_guard<std::mutex> guard(mutex);
		for (OpenALAudioSample* sample : samples)
            sample->pause();


	}

	void AudioManagerOpenAL::resume() {
		alive = true;
		std::lock_guard<std::mutex> guard(mutex);
		for (OpenALAudioSample* sample : samples)
            if(sample->isPaused())
                sample->pause();
	}


	bool Source::isPlaying()
	{
		int sourceState;
		alGetSourcei(sourceId, AL_SOURCE_STATE, &sourceState);
		return sourceState == AL_PLAYING;
	}

	void AudioManagerOpenAL::update()
	{
		if(!alive)
			return;
		std::lock_guard<std::mutex> guard(mutex);

		for (auto sample : samples)
			sample->update();

		for (auto sample : toDelete)
			delete (OpenALAudioSample*)sample;
		toDelete.clear();
	}

	void AudioManagerOpenAL::stopAllSounds()
	{
		mutex.lock();
		for (OpenALAudioSample* sample : samples)
		{
			mutex.unlock();
			sample->stop();
			mutex.lock();
		}
		mutex.unlock();

		std::lock_guard<std::mutex> guard(mutex);
		for(Source & source : sources)
		{
			alSourceStop(source.sourceId);

			ALint buffer = 0;
			alGetSourcei(source.sourceId, AL_BUFFER, &buffer);
			if(buffer > 0)
				alSourceUnqueueBuffers(source.sourceId, 1, (ALuint*)&buffer);

			source.lastSample = nullptr;
		}
	}

}
