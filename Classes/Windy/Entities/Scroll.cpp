#include "Scroll.h"

#include "./../GameTags.h"

using namespace windy;

bool Scroll::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(GameTags::General::Scroll);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    return true;
}

void Scroll::parseBehavior(const cocos2d::ValueMap& behavior) {
    std::string scrollType = behavior.at("type").asString();

    auto cameraScroll = CameraFlags::CameraScroll::ScrollRight;

    if (scrollType.compare("left") == 0) {
        cameraScroll = CameraFlags::CameraScroll::ScrollLeft;
    }

    this->scroll = cameraScroll;


}

void Scroll::onUpdate(float dt) {

}