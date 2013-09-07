#define _CRT_SECURE_NO_WARNINGS
#include "AudioManagerWindows.h"

#include <Windows.h>
#include <Mmsystem.h>

#include <blib/util/Log.h>


AudioManager* AudioManager::getInstance()
{
	static AudioManagerWindows* mgr = new AudioManagerWindows();
	return mgr;
}

void AudioManagerWindows::init()
{
}

void AudioManagerWindows::playMusic( std::string filename )
{
	mciSendString("close song1", NULL, 0, 0);
	mciSendString(("open ../android/assets/audio/music/"+filename+" type mpegvideo alias song1").c_str(), NULL, 0, 0); 
	mciSendString("play song1 repeat", NULL, 0, 0);
}

void AudioManagerWindows::stopMusic()
{
	mciSendString("stop song1", NULL, 0, 0);
}

static int i = 2;
void AudioManagerWindows::playSound( std::string filename )
{
	char buf[1024];
	int r = i;
	i++;
	if(i > 10000)
		i = 2;
	sprintf(buf, "open ../android/assets/audio/%s type mpegvideo alias audio%i", filename.c_str(), r);
	int ret = mciSendString(buf, NULL, 0, 0); 

	sprintf(buf, "play audio%i", r);
	ret = mciSendString(buf, NULL, 0, 0);
}
