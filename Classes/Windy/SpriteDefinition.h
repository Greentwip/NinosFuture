#ifndef SPRITE_DEFINITION_H
#define SPRITE_DEFINITION_H

#include <string>
#include "cocos2d.h"

namespace windy {
	class SpriteDefinition {
	public:
		SpriteDefinition();
		SpriteDefinition(cocos2d::Point anchor, cocos2d::Size size);
		SpriteDefinition(const std::string& dataFileName);
		SpriteDefinition(SpriteDefinition const& other);

		SpriteDefinition get(const std::string& name);

		cocos2d::Point pointFromString(const std::string& pointString);
		cocos2d::Size sizeFromString(const std::string& pointString);
	public:

		cocos2d::Point anchor;
		cocos2d::Size size;
		std::map<std::string, SpriteDefinition> definitions;
	};
}

#endif