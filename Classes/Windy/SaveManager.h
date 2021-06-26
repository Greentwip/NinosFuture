#ifndef WINDY_SAVE_MANAGER_H
#define WINDY_SAVE_MANAGER_H

#include "External/tao/json.hpp"

#include "cocos2d.h"

#include <fstream>

namespace windy {
	class Slot {

	};

	class SaveManager {
	public:
		static void saveSlot(int slot, const Slot& data) {

		}

		static Slot readSlot(int slot) {
			return Slot();
		}
	};
}


#endif
