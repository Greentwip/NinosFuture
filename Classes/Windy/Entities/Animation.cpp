#include "Animation.h"

#include "./../GameTags.h"

using namespace windy;

bool Animation::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->setTag(GameTags::Scenery::SpriteAnimation);

    this->setup();

    return true;
}


void Animation::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Animation::onUpdate(float dt) {

}