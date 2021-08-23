#include "Grenade.h"

#include "Windy/GameTags.h"

using namespace game;

game::Resources& Grenade::getResources() {
    static game::Resources resources{ game::ResourceKind::Weapon, "grenade_bullet" };
    return resources;
}

Grenade* Grenade::create() {
    auto entity = new (std::nothrow) Grenade();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;
}

std::shared_ptr<cocos2d::Rect> Grenade::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Grenade>(position, size);
}

bool Grenade::init() {
    if (!Node::init()) {
        return false;
    }

    return true;
}

void Grenade::setup() {
    power = 4;
    _riseSpeed = 6.0f;

    Logical::composite<Grenade>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "grenade_bullet_shoot", true, 0.10f);
    sprite->appendAction(action, false);
    sprite->runAction("shoot");

    ignoreLandscapeCollision = true;
    setTag(windy::GameTags::WeaponEnemy);
}

void Grenade::onFinished() {
    finishForever();
}

void Grenade::fire(bool flipped) {
    if (!flipped) {
        speed.x = -_moveSpeed;
    }
    else {
        speed.x = _moveSpeed;
    }

    if (!_rising) {
        speed.y = _riseSpeed;
        _rising = true;
    }
}

void Grenade::onUpdate(float dt) {

}
