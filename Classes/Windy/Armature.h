#ifndef __WINDY_ARMATURE_H__
#define __WINDY_ARMATURE_H__

#include <string>
#include <vector>

#include "cocos2d.h"

namespace windy {
	class Armature {
	public:
		Armature();
		Armature(cocos2d::Point anchor, std::vector<cocos2d::Rect> collisionRectangles);
		Armature(const std::string& dataFileName);
		Armature(Armature const& other);

		Armature get(const std::string& name);

		cocos2d::Point pointFromString(const std::string& pointString);
		cocos2d::Size sizeFromString(const std::string& pointString);
	public:
		cocos2d::Point anchor;
		std::vector<cocos2d::Rect> collisionRectangles;
		std::map<std::string, Armature> definitions;
	};
}

#endif