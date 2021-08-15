#include "Item.h"

#include "./../GameTags.h"

using namespace windy;

bool Item::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->id = -1;

    this->collectible = false;

    this->name = "none";

    this->setTag(GameTags::General::Item);

    return true;
}

void Item::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Item::onUpdate(float dt) {

}