#ifndef WINDY_SETTINGS_H
#define WINDY_SETTINGS_H

#include <map>
#include <string>

#include "ImageFormat.h"

namespace windy {

	class Settings;

	class SettingsData {
	public:
		float bgmVolume;
		float sfxVolume;

		std::map<std::string, float> soundAmplitudeValues;

	private:
		SettingsData(float bgmVolume, float sfxVolume, std::map<std::string, float> soundAmplitudeValues){
			this->bgmVolume = bgmVolume;
			this->sfxVolume = sfxVolume;
			this->soundAmplitudeValues = soundAmplitudeValues;
		}

		friend class Settings;
	};

	class Settings {
	public:
		static ImageFormat TextureFormat;
		static float bgmVolume;
		static float sfxVolume;
		static std::map<std::string, float> soundAmplitudeValues;


		static SettingsData read();
		static void load();
		static void save();
		static void apply();

	private:
		static std::map<std::string, float> loadDefaultAmplitudeValues();
	};
}

#endif
