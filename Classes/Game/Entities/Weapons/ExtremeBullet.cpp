#include "ExtremeBullet.h"

#include "Windy/AnimationAction.h"

using namespace game;

windy::Resources& ExtremeBullet::getResources() {
    static windy::Resources resources{windy::ResourceKind::Weapon, "extreme_bullet"};
    return resources;
}

ExtremeBullet* ExtremeBullet::create() {
    ExtremeBullet* entity = new (std::nothrow) ExtremeBullet();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;

}

bool ExtremeBullet::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void ExtremeBullet::setup() {
    Logical::composite<ExtremeBullet>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "extreme_bullet", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> ExtremeBullet::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<ExtremeBullet>(position, size);
}

void ExtremeBullet::onFinished() {
    this->finishForever();
}

void ExtremeBullet::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);

    float velocity = 4;

    this->speed = cocos2d::Point(static_cast<float>(velocity * direction), 0);

    this->setTag(tag);
}

void ExtremeBullet::onUpdate(float dt) {

}
