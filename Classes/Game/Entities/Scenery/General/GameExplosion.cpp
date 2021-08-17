#include "GameExplosion.h"

#include "Windy/AnimationAction.h"

#include <cmath>

using namespace game;

windy::Resources& GameExplosion::getResources() {
    static windy::Resources resources{windy::ResourceKind::LevelFX, "explosion"};
    return resources;
}

GameExplosion* GameExplosion::create() {
    GameExplosion* explosion = new (std::nothrow) GameExplosion();

    if (explosion && explosion->init()) {
        explosion->autorelease();
        return explosion;
    }

    CC_SAFE_DELETE(explosion);
    return nullptr;

}

bool GameExplosion::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(windy::GameTags::Scenery::Particle);


    return true;
}

void GameExplosion::setup(cocos2d::Color3B spriteColor) {
    Logical::composite<GameExplosion>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "explosion_death", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->sprite->setColor(spriteColor);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> GameExplosion::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<GameExplosion>(position, size);
}


void GameExplosion::fire(float angle) {

    //float deltaY = targetPosition.y - this->getPositionY();
    //float deltaX = targetPosition.x - this->getPositionX();

    //float angle = (float)std::atan2(deltaY, deltaX);

    float velocity = 2;
    float speedX = static_cast<float>(velocity * (float)std::cos(angle));
    float speedY = static_cast<float>(velocity * (float)std::sin(angle));
    this->speed = cocos2d::Point(speedX, speedY);

    this->setTag(windy::GameTags::Scenery::Particle);
}
