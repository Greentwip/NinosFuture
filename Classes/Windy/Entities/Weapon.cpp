#include "Weapon.h"

#include "./../Sprite.h"

using namespace windy;

bool Weapon::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}


void Weapon::parseBehavior(const cocos2d::ValueMap& behavior) {

}
