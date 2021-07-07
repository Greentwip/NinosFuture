#include "AudioManager.h"
#include "Settings.h"

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
int windy::AudioManager::currentBgmId = -1;

std::map<windy::Sounds, std::string> windy::AudioManager::soundsKeyMap = {
	{Sounds::Intro, "sounds/sfx_opening.mp3"},
	{Sounds::Abakura, "sounds/sfx_abakura.mp3"},
	{Sounds::Title, "sounds/bgm_title.mp3"},
	{Sounds::Select, "sounds/sfx_select.mp3"},
	{Sounds::Selected, "sounds/sfx_selected.mp3"},
	{Sounds::StageSelect, "sounds/bgm_stage_select.mp3"},
	{Sounds::COMBO_4, "audio/sfx/combo4.mp3"},
	{Sounds::COMBO_5, "audio/sfx/combo5.mp3"},
	{Sounds::DELICIOUS, "audio/sfx/delicious.mp3"}
};


int windy::AudioManager::playBgm(windy::Sounds resource, bool loop) {
	//cc.audio.current_track = path
	//local id = ccexp.AudioEngine:play2d(path, loop, cc.bgm_volume_)
	//cc.current_bgm_id_ = id
	//return id

	currentTrack = resource;

	AudioManager::currentBgmId = AudioEngine::play2d(soundsKeyMap[resource], loop, Settings::bgmVolume);

	return AudioManager::currentBgmId;
}

int windy::AudioManager::playSfx(windy::Sounds resource, bool loop) {
	/*if (!playAlong) {
	}*/
	
	//local id = ccexp.AudioEngine:play2d(path, loop, cc.sfx_volume_)
	//return id

	//currentSfxId = ;

	AudioManager::currentSfxId = AudioEngine::play2d(soundsKeyMap[resource], loop, Settings::sfxVolume);

	return AudioManager::currentSfxId;
}

void windy::AudioManager::stopAll() {
	AudioEngine::stopAll();
}

windy::Sounds windy::AudioManager::getCurrentTrack() {
	return currentTrack;
}

void windy::AudioManager::setBgmVolume(float volume) {
	AudioEngine::setVolume(AudioManager::currentBgmId, volume);
}

