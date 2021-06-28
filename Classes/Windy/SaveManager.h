#ifndef WINDY_SAVE_MANAGER_H
#define WINDY_SAVE_MANAGER_H

#include "External/tao/json.hpp"
#include <External/tao/json/contrib/traits.hpp>

#include "cocos2d.h"

#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

namespace windy {
	class SaveManager;

	class Slot {
	public:
		bool helmet;
		bool head;
		bool chest;
		bool fist;
		bool boot;

		std::vector<std::string> collectibles;

		int e;
		int m;

		int lives;

		bool freezer;
		bool sheriff;
		bool boomer;
		bool military;
		bool vine;
		bool shield;
		bool night;
		bool torch;


		bool helmetActivated;

	private:
		Slot() {
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

		friend class SaveManager;
	};

	class SaveManager {
	public:
		static Slot createSlot() {
			return Slot();
		}

		static bool slotExists(int slot) {
			std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();

			std::string slotPath = writablePath + "/" + "slot" + std::to_string(slot) + ".json";

			std::ifstream file(slotPath);

			return file.good();
		}

		static void saveSlot(int slot, const Slot& data) {

			std::map<std::string, std::string> collectibles;

			for (int i = 0; i < data.collectibles.size(); ++i) {
				collectibles[std::to_string(i)] = data.collectibles[i];
			}

			const tao::json::value jsonCollectibles = collectibles;


			tao::json::value v = {
				{"helmet", data.helmet},
				{"head", data.head},
				{"chest", data.chest},
				{"fist", data.fist},
				{"boot", data.boot},
				{"collectibles", jsonCollectibles},
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

		static Slot readSlot(int slot) {
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

				std::vector<std::string> collectibles;

				auto jsonCollectibles = v.as<std::map<std::string, std::string>>("collectibles");

				for (auto kv : jsonCollectibles) {
					collectibles.push_back(kv.second);
				}

				slot.collectibles = collectibles;
				
				slot.e = v.as<int>("e");
				slot.m = v.as<int>("m");

				slot.lives = v.as<int>("lives");

				slot.freezer = v.as<bool>("freezer");
				slot.sheriff = v.as<bool>("sheriff");
				slot.boomer = v.as<bool>("boomer");
				slot.military = v.as<bool>("military");
				slot.vine = v.as<bool>("vine");
				slot.shield= v.as<bool>("shield");
				slot.night = v.as<bool>("night");
				slot.torch = v.as<bool>("torch");

				slot.helmetActivated = v.as<bool>("helmetActivated");

				return slot;
			}
			else {
				return Slot();
			}
		}

		static void deleteSlot(int slot) {
			std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();

			std::string slotPath = writablePath + "/" + "slot" + std::to_string(slot) + ".json";
			
			std::remove(slotPath.c_str());
		}
	};
}


#endif
