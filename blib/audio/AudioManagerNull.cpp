#include "AudioManagerNull.h"


namespace blib
{

	AudioManager* AudioManager::getInstance()
	{
		static AudioManager* mgr = new AudioManagerNull();
		return mgr;
	}

	void AudioManagerNull::init()
	{
	}

	void AudioManagerNull::playMusic(std::string filename)
	{
	}

	void AudioManagerNull::stopMusic()
	{
	}

	AudioSample * AudioManagerNull::loadSample(const std::string & filename)
	{
		return new NullAudioSample();
	}

	void AudioManagerNull::playSound(std::string filename)
	{
	}

	void NullAudioSample::setVolume(int volume)
	{
	}

	void NullAudioSample::play(bool loop)
	{
	}

	void NullAudioSample::stop()
	{
	}

	bool NullAudioSample::isPlaying()
	{
		return false;
	}

}