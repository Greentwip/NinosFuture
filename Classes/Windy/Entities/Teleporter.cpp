#include "Teleporter.h"

#include "./../GameTags.h"

using namespace windy;

bool Teleporter::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(GameTags::General::Teleporter);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    return true;
}

void Teleporter::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Teleporter::onUpdate(float dt) {

}