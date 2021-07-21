#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <map>

#define USE_AUDIO_ENGINE 1
//#define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif


namespace windy {

	enum class Sounds {
		Intro,
		Abakura,
		Title,
		Select,
		Selected,
		StageSelect,
		BeltJoin,
		BossIntro,
		Door,
		BusterLow,
		BusterMid,
		BusterHigh, 
		BusterChargingMid,
		BusterChargingHigh,
		Land,
		NONE
	};


	class AudioManager {
	private:
		static std::map<Sounds, std::string> soundsKeyMap;

	public:
		static int playBgm(Sounds resource, bool loop = true);
		static int playSfx(Sounds resource, bool loop = false);
		static void stopAll();
		static cocos2d::experimental::AudioEngine::AudioState getState(int id);

		static Sounds getCurrentTrack();

		static void setBgmVolume(float volume);

	private:
		static Sounds currentTrack;
		static int currentBgmId;
		static int currentSfxId;
	};
}

#endif