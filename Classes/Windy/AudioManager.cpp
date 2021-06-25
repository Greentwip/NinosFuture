#include "AudioManager.h"

#include "cocos2d.h"

#define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

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


windy::Sounds windy::AudioManager::currentTrack = windy::Sounds::NONE;
int windy::AudioManager::currentSfxId = -1;

std::map<windy::Sounds, std::string> windy::AudioManager::soundsKeyMap = {
	{Sounds::Intro, "sounds/sfx_opening.mp3"},
	{Sounds::Abakura, "sounds/sfx_abakura.mp3"},
	{Sounds::Title, "sounds/bgm_title.mp3"},
	{Sounds::Select, "sounds/sfx_select.mp3"},
	{Sounds::Selected, "sounds/sfx_selected.mp3"},
	{Sounds::COMBO_3, "audio/sfx/combo3.mp3"},
	{Sounds::COMBO_4, "audio/sfx/combo4.mp3"},
	{Sounds::COMBO_5, "audio/sfx/combo5.mp3"},
	{Sounds::DELICIOUS, "audio/sfx/delicious.mp3"}
};


void windy::AudioManager::playBGM(windy::Sounds resource, bool loop) {
	//cc.audio.current_track = path
	//local id = ccexp.AudioEngine:play2d(path, loop, cc.bgm_volume_)
	//cc.current_bgm_id_ = id
	//return id

	currentTrack = resource;

	AudioEngine::play2d(soundsKeyMap[resource], loop);
}

void windy::AudioManager::playSFX(windy::Sounds resource, bool loop) {
	/*if (!playAlong) {
	}*/
	
	//local id = ccexp.AudioEngine:play2d(path, loop, cc.sfx_volume_)
	//return id

	//currentSfxId = ;

	AudioEngine::play2d(soundsKeyMap[resource], loop);
}

void windy::AudioManager::stopAll() {
	AudioEngine::stopAll();
}

windy::Sounds windy::AudioManager::getCurrentTrack() {
	return currentTrack;
}


