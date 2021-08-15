#ifndef WINDY_SAVE_MANAGER_H
#define WINDY_SAVE_MANAGER_H

#include "External/tao/json.hpp"
#include "External/tao/json/contrib/traits.hpp"

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

		std::vector<std::pair<std::string, bool> > collectibles;

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
		Slot();

		friend class SaveManager;
	};

	class SaveManager {
	public:
		static Slot createSlot();
		static bool slotExists(int slot);
		static void saveSlot(int slot, const Slot& data);
		static Slot readSlot(int slot);
		static void deleteSlot(int slot);
		static int defaultSlot;
	};

}


#endif
