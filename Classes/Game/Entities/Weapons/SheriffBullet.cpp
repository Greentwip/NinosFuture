#include "SheriffBullet.h"

#include "Windy/AnimationAction.h"

using namespace game;

game::Resources& SheriffBullet::getResources() {
    static game::Resources resources{game::ResourceKind::Weapon, "directional_star_bullet"};
    return resources;
}

SheriffBullet* SheriffBullet::create() {
    SheriffBullet* entity = new (std::nothrow) SheriffBullet();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;

}

bool SheriffBullet::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void SheriffBullet::setup() {
    Logical::composite<SheriffBullet>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "directional_star_bullet", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> SheriffBullet::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<SheriffBullet>(position, size);
}

void SheriffBullet::onFinished() {
    this->finishForever();
}



void SheriffBullet::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);

    float velocity = 4;

    this->speed = cocos2d::Point(static_cast<float>(velocity * direction), 0);

    this->setTag(tag);
}

void SheriffBullet::onUpdate(float dt) {

}
