#pragma once

#include <pewpew/AudioManager.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>


class AAssetManager;
class AAsset;


class AudioManagerAndroid : public AudioManager
{
	// engine interfaces
	SLObjectItf engineObject;
	SLEngineItf engineEngine;

	// output mix interfaces
	SLObjectItf outputMixObject;
	SLEnvironmentalReverbItf outputMixEnvironmentalReverb;

	// file descriptor player interfaces
	SLObjectItf fdPlayerObject;
	SLPlayItf fdPlayerPlay;
	SLSeekItf fdPlayerSeek;
	SLMuteSoloItf fdPlayerMuteSolo;
	SLVolumeItf fdPlayerVolume;


	bool playingMusic;

	AAssetManager* mgr;
public:
	virtual ~AudioManagerAndroid() {};
	virtual void setAssetManager(AAssetManager* mgr);
	virtual void init();
	virtual void playMusic( std::string filename );
	virtual void stopMusic();
	virtual void playSound( std::string filename );
};
