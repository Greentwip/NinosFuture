#ifndef WINDY_SETTINGS_H
#define WINDY_SETTINGS_H

#include "ImageFormat.h"


namespace windy {

	class Settings;

	class SettingsData {
	public:
		float bgmVolume;
		float sfxVolume;

	private:
		SettingsData(float bgmVolume = 1.0f, float sfxVolume = 1.0f){
			this->bgmVolume = bgmVolume;
			this->sfxVolume = sfxVolume;
		}

		friend class Settings;
	};

	class Settings {
	public:
		static ImageFormat TextureFormat;
		static float bgmVolume;
		static float sfxVolume;

		static SettingsData read();
		static void load();
		static void save();
		static void apply();
	};
}

#endif
