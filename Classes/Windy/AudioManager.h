#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <map>

namespace windy {

	enum class Sounds {
		Intro,
		Abakura,
		Title,
		Select,
		Selected,
		StageSelect,
		COMBO_1,
		COMBO_2,
		COMBO_3,
		COMBO_4,
		COMBO_5,
		DELICIOUS,
		NONE
	};


	class AudioManager {
	private:
		static std::map<Sounds, std::string> soundsKeyMap;

	public:
		static int playBgm(Sounds, bool loop = true);
		static int playSfx(Sounds resource, bool loop = false);
		static void stopAll();
		static Sounds getCurrentTrack();

		static void setBgmVolume(float volume);

	private:
		static Sounds currentTrack;
		static int currentBgmId;
		static int currentSfxId;
	};
}

#endif