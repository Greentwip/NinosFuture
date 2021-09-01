#include "Settings.h"
#include "AudioManager.h"

#include "External/tao/json.hpp"
#include "External/tao/json/contrib/traits.hpp"

#include "cocos2d.h"

#include <fstream>


windy::ImageFormat windy::Settings::TextureFormat = windy::ImageFormat::PVR;

float windy::Settings::bgmVolume = 1.0f;
float windy::Settings::sfxVolume = 1.0f;

std::map<std::string, float> windy::Settings::soundAmplitudeValues;

windy::SettingsData windy::Settings::read() {
	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
	std::string settingsPath = writablePath + "/" + "options.json";

    if (std::filesystem::exists(settingsPath)) {

        const tao::json::value v = tao::json::from_file(settingsPath);

		float bgmVolume = v.as<float>("bgmVolume");
		float sfxVolume = v.as<float>("sfxVolume");

		if (v.find("soundAmplitudeValues")) {
			auto amplitudeValues = v.as<std::map<std::string, float>>("soundAmplitudeValues");
			return SettingsData(bgmVolume, sfxVolume, amplitudeValues);
		}
		else {
			return SettingsData(bgmVolume, sfxVolume, loadDefaultAmplitudeValues());
		}

		
	}
	else {
		return SettingsData(1.0f, 1.0f, loadDefaultAmplitudeValues());
	}
}


void windy::Settings::load() {
	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();

	std::string settingsPath = writablePath + "/" + "options.json";


	if (std::filesystem::exists(settingsPath)) {

		const tao::json::value v = tao::json::from_file(settingsPath);

		Settings::bgmVolume = v.as<float>("bgmVolume");
		Settings::sfxVolume = v.as<float>("sfxVolume");

		if (v.find("soundAmplitudeValues")) {
			auto amplitudeValues = v.as<std::map<std::string, float>>("soundAmplitudeValues");
			Settings::soundAmplitudeValues = amplitudeValues;
		}
		else {
			Settings::soundAmplitudeValues = loadDefaultAmplitudeValues();
		}

	}
	else {
		Settings::bgmVolume = 1.0f;
		Settings::sfxVolume = 1.0f;
		Settings::soundAmplitudeValues = loadDefaultAmplitudeValues();
	}

	
}

void windy::Settings::save() {
	const tao::json::value amplitudeValues = soundAmplitudeValues;

	tao::json::value v = {
		{"bgmVolume", bgmVolume},
		{"sfxVolume", sfxVolume},
		{"soundAmplitudeValues", amplitudeValues}
	};

	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
	std::string settingsPath = writablePath + "/" + "options.json";

	std::ofstream out(settingsPath);

	tao::json::to_stream(out, v);
	out.close();
}

void windy::Settings::apply() {
	AudioManager::setBgmVolume(Settings::bgmVolume);
}


std::map<std::string, float> windy::Settings::loadDefaultAmplitudeValues() {
	std::map<std::string, float> amplitudes;

	auto soundKeyMap = windy::AudioManager::getSoundKeyMap();

	for (int i = static_cast<int>(windy::Sounds::Intro); i <= static_cast<int>(windy::Sounds::GetWeapon); ++i) {
		amplitudes[soundKeyMap[static_cast<windy::Sounds>(i)]] = 1.0f;
	}

	return amplitudes;

}