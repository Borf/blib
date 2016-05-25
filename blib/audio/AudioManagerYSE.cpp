#include "AudioManagerYSE.h"


#include <blib/util/Log.h>
using blib::util::Log;

namespace blib
{
	AudioManager* AudioManager::getInstance()
	{
		static AudioManager* mgr = new AudioManagerYSE();
		return mgr;
	}


	AudioManagerYSE::AudioManagerYSE()
	{
		YSE::System().init();
	}

	AudioManagerYSE::~AudioManagerYSE()
	{
		YSE::System().close();
	}

	void AudioManagerYSE::init()
	{

	}

	void AudioManagerYSE::update()
	{
		YSE::System().update();
	}

	AudioSample* AudioManagerYSE::loadSample(const std::string &filename)
	{
		return new YSEAudioSample(filename);
	}


	YSEAudioSample::YSEAudioSample(const std::string &fileName)
	{
		sound.create(("../" + fileName).c_str(), nullptr, false, 1.0f, true);
		if (!sound.isValid()) {
			Log::out << "sound 'drone.ogg' not found" << Log::newline;
		}
		sound.stop();
	}

	void YSEAudioSample::play(bool loop)
	{
		sound.setLooping(loop);
		sound.play();
	}

	void YSEAudioSample::stop()
	{
		sound.stop();
	}

}