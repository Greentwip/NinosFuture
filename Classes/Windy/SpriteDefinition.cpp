#include "SpriteDefinition.h"

#include <regex>
#include <vector>

using namespace windy;

SpriteDefinition::SpriteDefinition(): anchor(cocos2d::Point(0, 0)), size(cocos2d::Size(0, 0)){

}

SpriteDefinition::SpriteDefinition(cocos2d::Point anchor, cocos2d::Size size) {
    this->anchor = anchor;
    this->size = size;
}

SpriteDefinition::SpriteDefinition(const std::string& dataFileName) {
    auto plistDict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(dataFileName + ".plist");

    for (auto kp : plistDict["definitions"].asValueMap()) {
        auto definitionName = kp.first;
        auto definition = kp.second.asValueMap();

        auto definitionToStore = SpriteDefinition(this->pointFromString(definition["anchor"].asString()), 
                                                  this->sizeFromString(definition["size"].asString()));
        this->definitions[definitionName] = definitionToStore;
    }
}


cocos2d::Point SpriteDefinition::pointFromString(const std::string& pointString) {
    std::vector<int> stringPoints;

    std::regex e("(-*[0-9]+.[0-9]+)");

    std::sregex_token_iterator iter(pointString.begin(), pointString.end(), e, -1);
    std::sregex_token_iterator end;

    for (; iter != end; ++iter) {
        stringPoints.push_back(std::atoi(iter->str().c_str()));
    }

    return cocos2d::Point((float)stringPoints[0], (float)stringPoints[1]);
}

SpriteDefinition::SpriteDefinition(SpriteDefinition const& other) {
    this->anchor = other.anchor;
    this->size = other.size;
}

cocos2d::Size SpriteDefinition::sizeFromString(const std::string& sizeString) {
    auto point = this->pointFromString(sizeString);
    return cocos2d::Size(point.x, point.y);
}

SpriteDefinition SpriteDefinition::get(const std::string& name) {
    return this->definitions[name];
}

