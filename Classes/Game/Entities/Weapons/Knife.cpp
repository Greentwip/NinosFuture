#include "Knife.h"

#include "Windy/AnimationAction.h"

#include <cmath>

using namespace game;

game::Resources& Knife::getResources() {
    static game::Resources resources{ game::ResourceKind::Weapon, "claw_bullet" };
    return resources;
}

Knife* Knife::create() {
    Knife* entity = new (std::nothrow) Knife();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;

}

bool Knife::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void Knife::setup() {
    Logical::composite<Knife>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "claw_bullet_shoot", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

    this->rotationCounter = 0;

}

std::shared_ptr<cocos2d::Rect> Knife::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Knife>(position, size);
}

void Knife::onFinished() {
    this->finishForever();
}

void Knife::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);

    float velocity = 2;

    this->speed = cocos2d::Point(static_cast<float>(velocity * direction), 0);

    this->setTag(tag);
}

void Knife::fire(int power, const cocos2d::Point& playerPosition, windy::GameTags::Weapon tag) {

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

void Knife::onUpdate(float dt) {
    this->setRotation(static_cast<float>(this->rotationCounter));
    this->rotationCounter += 18;
}
