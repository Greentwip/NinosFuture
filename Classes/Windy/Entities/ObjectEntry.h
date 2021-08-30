#ifndef __WINDY_OBJECT_ENTRY_H__
#define __WINDY_OBJECT_ENTRY_H__

#include <memory>
#include <functional>

#include "cocos2d.h"

namespace windy {
	class Logical;
}

namespace windy {
	class ObjectEntry {
	public:
		ObjectEntry(std::shared_ptr<cocos2d::Rect> collisionRectangle, std::function<Logical* ()> createFunction, int priority = 0) {
			this->collisionRectangle = collisionRectangle;
			this->createFunction = createFunction;
			this->mappedInstance = nullptr;

			this->finished = false;
			this->finishedForever = false;
			this->respawnPrevention = false;

			this->priority = priority;
		}

		std::shared_ptr<cocos2d::Rect> collisionRectangle;

		std::function<Logical* ()> createFunction;

		Logical* mappedInstance;

		bool finished;
		bool finishedForever;
		bool respawnPrevention;

		int priority;
	};
}

#endif