#pragma once

#include "AudioManager.h"

namespace blib
{
	class AudioManagerWindows : public AudioManager
	{
	public:
		virtual void init();
		virtual void playMusic( std::string filename );
		virtual void stopMusic();
		virtual void playSound( std::string filename );
	};
}
