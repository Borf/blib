#include <blib/audio/AudioManagerAndroid.h>
#include <blib/util/Log.h>

#include <assert.h>
// for native asset manager
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

using blib::util::Log;

namespace blib
{

	AudioManager* AudioManager::getInstance()
	{
		static AudioManagerAndroid* mgr = new AudioManagerAndroid();
		return mgr;
	}

	void AudioManagerAndroid::init()
	{
		playingMusic = false;
		engineObject = NULL;
		outputMixObject = NULL;
		outputMixEnvironmentalReverb = NULL;
		fdPlayerObject = NULL;


		SLresult result;
		// create engine
		result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
		assert(SL_RESULT_SUCCESS == result);
		result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == result);
		result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
		assert(SL_RESULT_SUCCESS == result);

		const SLInterfaceID ids[1] = { SL_IID_ENVIRONMENTALREVERB };
		const SLboolean req[1] = { SL_BOOLEAN_FALSE };
		result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
		assert(SL_RESULT_SUCCESS == result);
		result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == result);
	}

	void AudioManagerAndroid::setAssetManager(AAssetManager* mgr)
	{
		this->mgr = mgr;
	}

	void AudioManagerAndroid::playMusic(std::string filename)
	{
		if (playingMusic)
			stopMusic();
		SLresult result;

		AAsset* asset = AAssetManager_open(mgr, ("audio/music/" + filename).c_str(), AASSET_MODE_UNKNOWN);
		if (NULL == asset) {
			return;
		}
		off_t start, length;
		int fd = AAsset_openFileDescriptor(asset, &start, &length);
		assert(0 <= fd);
		AAsset_close(asset);

		// configure audio source
		SLDataLocator_AndroidFD loc_fd = { SL_DATALOCATOR_ANDROIDFD, fd, start, length };
		SLDataFormat_MIME format_mime = { SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };
		SLDataSource audioSrc = { &loc_fd, &format_mime };

		Log::out << "Configure Audio Sink" << Log::newline;
		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, outputMixObject };
		SLDataSink audioSnk = { &loc_outmix, NULL };

		Log::out << "Create Audio Player" << Log::newline;
		// create audio player
		const SLInterfaceID ids_[1] = { SL_IID_SEEK };
		const SLboolean req_[1] = { SL_BOOLEAN_TRUE };
		result = (*engineEngine)->CreateAudioPlayer(engineEngine, &fdPlayerObject, &audioSrc, &audioSnk, 1, ids_, req_);
		assert(SL_RESULT_SUCCESS == result);
		result = (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == result);
		result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY, &fdPlayerPlay);
		assert(SL_RESULT_SUCCESS == result);
		result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_SEEK, &fdPlayerSeek);
		assert(SL_RESULT_SUCCESS == result);
		result = (*fdPlayerSeek)->SetLoop(fdPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
		assert(SL_RESULT_SUCCESS == result);

		if (NULL != fdPlayerPlay) {

			// set the player's state
			result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
			assert(SL_RESULT_SUCCESS == result);
			(void)result;
			playingMusic = true;
		}

	}

	void AudioManagerAndroid::stopMusic()
	{
		SLresult result;
		if (NULL != fdPlayerPlay) {

			// set the player's state
			result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_STOPPED);
			assert(SL_RESULT_SUCCESS == result);

			playingMusic = false;
		}
	}

	void AudioManagerAndroid::playSound(std::string filename)
	{
		SLresult result;

		AAsset* asset = AAssetManager_open(mgr, ("audio/" + filename).c_str(), AASSET_MODE_UNKNOWN);
		if (NULL == asset) {
			return;
		}
		off_t start, length;
		int fd = AAsset_openFileDescriptor(asset, &start, &length);
		assert(0 <= fd);
		AAsset_close(asset);

		// configure audio source
		SLDataLocator_AndroidFD loc_fd = { SL_DATALOCATOR_ANDROIDFD, fd, start, length };
		SLDataFormat_MIME format_mime = { SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };
		SLDataSource audioSrc = { &loc_fd, &format_mime };

		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, outputMixObject };
		SLDataSink audioSnk = { &loc_outmix, NULL };

		// create audio player
		const SLInterfaceID ids_[0] = {};
		const SLboolean req_[0] = {};
		result = (*engineEngine)->CreateAudioPlayer(engineEngine, &fdPlayerObject, &audioSrc, &audioSnk, 0, ids_, req_);
		assert(SL_RESULT_SUCCESS == result);
		result = (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == result);
		result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY, &fdPlayerPlay);
		assert(SL_RESULT_SUCCESS == result);

		if (NULL != fdPlayerPlay) {
			result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
			assert(SL_RESULT_SUCCESS == result);
			(void)result;
		}
	}

	class AndroidAudioSample : public AudioSample
	{
	public:
		std::string fileName;
		AudioManagerAndroid* manager;
		AndroidAudioSample(const std::string &fileName, AudioManagerAndroid* manager)
		{
			this->fileName = fileName;
			this->manager = manager;
		}

		virtual void play(bool loop = false) override
		{
			SLresult result;

			AAsset* asset = AAssetManager_open(manager->mgr, fileName.c_str(), AASSET_MODE_UNKNOWN);
			if (NULL == asset) {
				return;
			}
			off_t start, length;
			int fd = AAsset_openFileDescriptor(asset, &start, &length);
			assert(0 <= fd);
			AAsset_close(asset);

			// configure audio source
			SLDataLocator_AndroidFD loc_fd = { SL_DATALOCATOR_ANDROIDFD, fd, start, length };
			SLDataFormat_MIME format_mime = { SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };
			SLDataSource audioSrc = { &loc_fd, &format_mime };

			// configure audio sink
			SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, manager->outputMixObject };
			SLDataSink audioSnk = { &loc_outmix, NULL };

			// create audio player
			const SLInterfaceID ids_[0] = {};
			const SLboolean req_[0] = {};
			result = (*manager->engineEngine)->CreateAudioPlayer(manager->engineEngine, &manager->fdPlayerObject, &audioSrc, &audioSnk, 0, ids_, req_);
			assert(SL_RESULT_SUCCESS == result);
			result = (*manager->fdPlayerObject)->Realize(manager->fdPlayerObject, SL_BOOLEAN_FALSE);
			assert(SL_RESULT_SUCCESS == result);
			result = (*manager->fdPlayerObject)->GetInterface(manager->fdPlayerObject, SL_IID_PLAY, &manager->fdPlayerPlay);
			assert(SL_RESULT_SUCCESS == result);

			if (NULL != manager->fdPlayerPlay) {
				result = (*manager->fdPlayerPlay)->SetPlayState(manager->fdPlayerPlay, SL_PLAYSTATE_PLAYING);
				assert(SL_RESULT_SUCCESS == result);
				(void)result;
			}
		}

		virtual void stop() override
		{
		}
        virtual bool isPlaying() override
        {
            return false;
        }
        virtual void setVolume(int volume) override {/*TODO*/ }

    };

	AudioSample* AudioManagerAndroid::loadSample(const std::string &filename)
	{
		return new AndroidAudioSample(filename, this);
	}

}