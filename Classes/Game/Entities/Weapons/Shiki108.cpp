#include "Shiki108.h"

#include "Windy/AnimationAction.h"
#include "Windy/Entities/ObjectEntry.h"

using namespace game;

game::Resources& Shiki108::getResources() {
    static game::Resources resources{ game::ResourceKind::Weapon, "sliding_flame_bullet" };
    return resources;
}

Shiki108* Shiki108::create() {
    Shiki108* entity = new (std::nothrow) Shiki108();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;

}

bool Shiki108::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void Shiki108::setup() {
    Logical::composite<Shiki108>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "sliding_flame_bullet_shoot", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");
}


std::shared_ptr<cocos2d::Rect> Shiki108::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Shiki108>(position, size);
}

void Shiki108::onFinished() {
    this->finishForever();
}

void Shiki108::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);

    float velocity = 4;

    this->speed = cocos2d::Point(static_cast<float>(velocity * direction), 0);

    this->setTag(tag);
}

void Shiki108::onUpdate(float dt) {
    if (this->contacts[windy::CollisionContact::Left] || this->contacts[windy::CollisionContact::Right]) {
        if (!this->entry->finishedForever) {
            this->finishForever();
        }
    }
}
