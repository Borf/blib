#include "AudioManagerWindows.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <Mmsystem.h>

#include <blib/util/Log.h>

#include <direct.h>

namespace blib
{
#if 0
	AudioManager* AudioManager::getInstance()
	{
		static AudioManagerWindows* mgr = new AudioManagerWindows();
		return mgr;
	}
#endif

	void AudioManagerWindows::init()
	{
	}

	void AudioManagerWindows::playMusic( std::string filename )
	{
/*		mciSendString("close song1", NULL, 0, 0);
		mciSendString(("open "+filename+" type mpegvideo alias song1").c_str(), NULL, 0, 0); 
		mciSendString("play song1 repeat", NULL, 0, 0);*/
	}

	void AudioManagerWindows::stopMusic()
	{
	//	mciSendString("stop song1", NULL, 0, 0);
	}

	static int i = 2;
	void AudioManagerWindows::playSound( std::string filename )
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
}