#include "Settings.h"
#include "AudioManager.h"

#include "External/tao/json.hpp"

#include "cocos2d.h"

#include <fstream>


windy::ImageFormat windy::Settings::TextureFormat = windy::ImageFormat::PVR;

float windy::Settings::bgmVolume = 1.0f;
float windy::Settings::sfxVolume = 1.0f;

windy::SettingsData windy::Settings::read() {
	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
	std::string settingsPath = writablePath + "/" + "options.json";

    if (std::filesystem::exists(settingsPath)) {

        const tao::json::value v = tao::json::from_file(settingsPath);

		float bgmVolume = v.as<float>("bgmVolume");
		float sfxVolume = v.as<float>("sfxVolume");

		return SettingsData(bgmVolume, sfxVolume);
	}
	else {
		return SettingsData();
	}

	

}

void windy::Settings::load() {
	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();

	std::string settingsPath = writablePath + "/" + "options.json";


	if (std::filesystem::exists(settingsPath)) {

		const tao::json::value v = tao::json::from_file(settingsPath);

		Settings::bgmVolume = v.as<float>("bgmVolume");
		Settings::sfxVolume = v.as<float>("sfxVolume");
	}
	else {
		Settings::bgmVolume = 1.0f;
		Settings::sfxVolume = 1.0f;
	}

	
}

void windy::Settings::save() {
	tao::json::value v = {
		{"bgmVolume", bgmVolume},
		{"sfxVolume", sfxVolume}
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
