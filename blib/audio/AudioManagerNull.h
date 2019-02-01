#pragma once

#include "AudioManager.h"

namespace blib
{
	class NullAudioSample : public AudioSample
	{
	public:
		// Inherited via AudioSample
		virtual void setVolume(int volume) override;
		virtual void play(bool loop = false) override;
		virtual void stop() override;
		virtual bool isPlaying() override;
	};

	class AudioManagerNull : public AudioManager
	{
	public:
		// Inherited via AudioManager
		virtual void init() override;
		virtual void playMusic(std::string filename) override;
		virtual void stopMusic() override;
		virtual AudioSample * loadSample(const std::string & filename) override;
		virtual void playSound(std::string filename) override;
	};
}