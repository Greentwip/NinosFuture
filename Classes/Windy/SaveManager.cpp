#include "SaveManager.h"

using namespace windy;

Slot::Slot() {
	helmet = false;
	head = false;
	chest = false;
	fist = false;
	boot = false;
	collectibles.clear();

	e = 0;
	m = 0;

	lives = 3;

	freezer = false;
	sheriff = false;
	boomer = false;
	military = false;
	vine = false;
	shield = false;
	night = false;
	torch = false;

	helmetActivated = false;
}

int SaveManager::defaultSlot = 0;

Slot SaveManager::createSlot() {
	return Slot();
}

bool SaveManager::slotExists(int slot) {
	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();

	std::string slotPath = writablePath + "/" + "slot" + std::to_string(slot) + ".json";

	std::ifstream file(slotPath);

	return file.good();
}

void SaveManager::saveSlot(int slot, const Slot& data) {

	tao::json::value v = {
		{"helmet", data.helmet},
		{"head", data.head},
		{"chest", data.chest},
		{"fist", data.fist},
		{"boot", data.boot},
		{"collectibles", data.collectibles},
		{"e", data.e},
		{"m", data.m},
		{"lives", data.lives},
		{"freezer", data.freezer},
		{"sheriff", data.sheriff},
		{"boomer", data.boomer},
		{"military", data.military},
		{"vine", data.vine},
		{"shield", data.shield},
		{"night", data.night},
		{"torch", data.torch},
		{"helmetActivated", data.helmetActivated}
	};
	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();

	std::string slotPath = writablePath + "/" + "slot" + std::to_string(slot) + ".json";

	std::ofstream out(slotPath);

	tao::json::to_stream(out, v);
	out.close();

}


Slot SaveManager::readSlot(int slot) {
	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
	std::string slotPath = writablePath + "/" + "slot" + std::to_string(slot) + ".json";

	std::ifstream file(slotPath);

	if (file.good()) {
		const tao::json::value v = tao::json::from_file(slotPath);

		Slot slot;

		slot.helmet = v.as<bool>("helmet");
		slot.head = v.as<bool>("head");
		slot.chest = v.as<bool>("chest");
		slot.fist = v.as<bool>("fist");
		slot.boot = v.as<bool>("boot");

		auto jsonCollectibles = v.as<std::vector<std::pair<std::string, bool>>>("collectibles");

		slot.collectibles = jsonCollectibles;

		slot.e = v.as<int>("e");
		slot.m = v.as<int>("m");

		slot.lives = v.as<int>("lives");

		slot.freezer = v.as<bool>("freezer");
		slot.sheriff = v.as<bool>("sheriff");
		slot.boomer = v.as<bool>("boomer");
		slot.military = v.as<bool>("military");
		slot.vine = v.as<bool>("vine");
		slot.shield = v.as<bool>("shield");
		slot.night = v.as<bool>("night");
		slot.torch = v.as<bool>("torch");

		slot.helmetActivated = v.as<bool>("helmetActivated");

		return slot;
	}
	else {
		return Slot();
	}
}

void SaveManager::deleteSlot(int slot) {
	std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();

	std::string slotPath = writablePath + "/" + "slot" + std::to_string(slot) + ".json";

	std::remove(slotPath.c_str());
}