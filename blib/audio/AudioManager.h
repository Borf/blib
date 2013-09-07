#pragma once

#include <string>

class AudioManager
{
public:
	AudioManager() {};
	virtual ~AudioManager() {};
	static AudioManager* getInstance(); // is implemented in AudioManagerWindows.cpp and AudioManagerAndroid.cpp

	virtual void init() = 0;
	virtual void playMusic(std::string filename) = 0;
	virtual void stopMusic() = 0;
	virtual void playSound(std::string filename) = 0;
};
