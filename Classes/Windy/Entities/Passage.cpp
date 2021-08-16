#include "Passage.h"
#include "./../GameTags.h"

using namespace windy;

bool Passage::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(GameTags::General::Hole);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    return true;
}


void Passage::parseBehavior(const cocos2d::ValueMap& behavior) {

}

std::shared_ptr<cocos2d::Rect> Passage::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return std::make_shared<cocos2d::Rect>(position.x - size.width * 0.5f, position.y - size.height * 0.5f, size.width, size.height);
}


void Passage::onUpdate(float dt) {

}