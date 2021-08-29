#include "MilitaryMissile.h"

#include "Windy/AnimationAction.h"

using namespace game;

game::Resources& MilitaryMissile::getResources() {
    static game::Resources resources{ game::ResourceKind::Weapon, "linear_missile_bullet" };
    return resources;
}

MilitaryMissile* MilitaryMissile::create() {
    MilitaryMissile* entity = new (std::nothrow) MilitaryMissile();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;

}

bool MilitaryMissile::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void MilitaryMissile::setup() {
    Logical::composite<MilitaryMissile>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "linear_missile_bullet_shoot", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> MilitaryMissile::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<MilitaryMissile>(position, size);
}

void MilitaryMissile::onFinished() {
    this->finishForever();
}

void MilitaryMissile::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);

    float velocity = 4;

    this->speed = cocos2d::Point(static_cast<float>(velocity * direction), 0);

    this->setTag(tag);
}

void MilitaryMissile::onUpdate(float dt) {

}
