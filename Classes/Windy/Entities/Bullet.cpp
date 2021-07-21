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

void Bullet::fire(int power, int direction, GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);
    this->speed = cocos2d::Point(static_cast<float>(6 * direction), 0);

    this->setTag(tag);
}