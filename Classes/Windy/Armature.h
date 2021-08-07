#ifndef __WINDY_ARMATURE_H__
#define __WINDY_ARMATURE_H__

#include <string>
#include <vector>
#include <memory>

#include "cocos2d.h"

namespace windy {
	class Armature {
	public:
		static void clearPlistCache();
		static void cache(const std::string& dataFileName);

		Armature();
		Armature(cocos2d::Point anchor, std::vector<std::shared_ptr<cocos2d::Rect>> collisionRectangles);
		Armature(const std::string& dataFileName);
		Armature(Armature const& other);

		Armature get(const std::string& name);

		static cocos2d::Point pointFromString(const std::string& pointString);
		static cocos2d::Size sizeFromString(const std::string& pointString);
	public:
		cocos2d::Point anchor;
		std::vector<std::shared_ptr<cocos2d::Rect> > collisionRectangles;
		std::map<std::string, Armature> definitions;
	};
}

#endif