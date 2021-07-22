#include "LandscapeTile.h"

using namespace windy;

LandscapeTile* LandscapeTile::create(cocos2d::Sprite* sprite, const cocos2d::Point& position, const cocos2d::Size& size) {
    LandscapeTile* landscapeTile = new (std::nothrow) LandscapeTile();

    if (landscapeTile) {
        landscapeTile->sprite = sprite;
        landscapeTile->setup(position, size);
    }

    if (landscapeTile && landscapeTile->init()) {
        landscapeTile->autorelease();
        return landscapeTile;
    }

    CC_SAFE_DELETE(landscapeTile);
    return nullptr;

}

bool LandscapeTile::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->addChild(this->sprite, 256);

    return true;
}

void LandscapeTile::setup(const cocos2d::Point& position, const cocos2d::Size& size) {
    this->setPosition(position);
    this->collisionBox = std::make_shared<cocos2d::Rect>(position.x, position.y + size.height, size.width, size.height);
}

