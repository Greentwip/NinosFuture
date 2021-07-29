#include "Bounds.h"

#include "./../GameTags.h"

using namespace windy;

bool Bounds::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }


    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    this->collisionRectangles[0]->origin = cocos2d::Point(-visibleSize.width * 0.5f, -visibleSize.height * 0.5f);
    this->collisionRectangles[0]->size = visibleSize;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];

    this->setTag(GameTags::General::Bounds);

    return true;
}

cocos2d::Rect Bounds::inflate(cocos2d::Size size) {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();    
    auto inflatedCollisionRectangle = *this->collisionRectangles[0];

    //inflatedCollisionRectangle.origin = cocos2d::Point(-visibleSize.width * 0.5f - size.width * 0.5f, -visibleSize.height * 0.5f - size.height * 0.5f);
    inflatedCollisionRectangle.origin -= cocos2d::Point(size.width, size.height);
    inflatedCollisionRectangle.size = visibleSize + size * 2;

    //inflatedCollisionRectangle = *Logical::normalizeCollisionRectangle(this, inflatedCollisionRectangle);

    return inflatedCollisionRectangle;
}

int Bounds::top() {
    return static_cast<int>(collisionRectangles[0]->getMaxY());
}

int Bounds::bottom() {
    return static_cast<int>(collisionRectangles[0]->getMinY());
}

int Bounds::left() {
    return static_cast<int>(collisionRectangles[0]->getMinX());
}

int Bounds::right() {
    return static_cast<int>(collisionRectangles[0]->getMaxX());
}

cocos2d::Point Bounds::center() {
    return cocos2d::Point(collisionRectangles[0]->getMidX(), collisionRectangles[0]->getMidY());
}


void Bounds::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Bounds::onUpdate(float dt) {

}