#include "Bullet.h"

#include "./../Sprite.h"

using namespace windy;

bool Bullet::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}


void Bullet::parseBehavior(const cocos2d::ValueMap& behavior) {

}
