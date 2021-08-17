#include "DirectionalBullet.h"

#include "Windy/AnimationAction.h"

#include <cmath>

using namespace game;

windy::Resources& DirectionalBullet::getResources() {
    static windy::Resources resources{windy::ResourceKind::Weapon, "directional_bullet"};
    return resources;
}

DirectionalBullet* DirectionalBullet::create() {
    DirectionalBullet* directionalBullet = new (std::nothrow) DirectionalBullet();

    if (directionalBullet && directionalBullet->init()) {
        directionalBullet->autorelease();
        return directionalBullet;
    }

    CC_SAFE_DELETE(directionalBullet);
    return nullptr;

}

bool DirectionalBullet::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void DirectionalBullet::setup() {
    Logical::composite<DirectionalBullet>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "directional_bullet", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}

std::shared_ptr<cocos2d::Rect> DirectionalBullet::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<DirectionalBullet>(position, size);
}

void DirectionalBullet::onFinished() {
    this->finishForever();
}

void DirectionalBullet::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);

    float velocity = 2;

    this->speed = cocos2d::Point(static_cast<float>(velocity * direction), 0);

    this->setTag(tag);
}

void DirectionalBullet::fire(int power, const cocos2d::Point& playerPosition, windy::GameTags::Weapon tag) {

    this->power = power;

    float deltaY = playerPosition.y - this->getPositionY();
    float deltaX = playerPosition.x - this->getPositionX();

    float angle = (float)std::atan2(deltaY, deltaX);

    float velocity = 2;
    float speedX = static_cast<float>(velocity * (float)std::cos(angle));
    float speedY = static_cast<float>(velocity * (float)std::sin(angle));
    this->speed = cocos2d::Point(speedX, speedY);

    this->setTag(tag);
}

void DirectionalBullet::onUpdate(float dt) {

}
