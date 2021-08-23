#include "BackPackBullet.h"

#include "Windy/GameTags.h"

using namespace game;

game::Resources& BackPackBullet::getResources() {
    static game::Resources resources{game::ResourceKind::Weapon, "backpacker_bullet"};
    return resources;
}

BackPackBullet* BackPackBullet::create() {
    auto entity = new (std::nothrow) BackPackBullet();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;
}

std::shared_ptr<cocos2d::Rect> BackPackBullet::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<BackPackBullet>(position, size);
}

bool BackPackBullet::init() {
    if (!Node::init()) {
        return false;
    }

    return true;
}

void BackPackBullet::setup() {
    power = 4;
    _moveSpeed = 2.0f;

    Logical::composite<BackPackBullet>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "backpacker_bullet_shoot", true, 0.10f);
    sprite->appendAction(action, false);
    sprite->runAction("shoot");

    ignoreLandscapeCollision = true;
    setTag(windy::GameTags::WeaponEnemy);
}

void BackPackBullet::onFinished() {
    finishForever();
}

void BackPackBullet::fire(bool flipped) {
    if (!flipped) {
        speed.x = -_moveSpeed;
    } else {
        speed.x = _moveSpeed;
    }

    if (!_rising) {
        speed.y = _riseSpeed;
        _rising = true;
    }

    if (!flipped) {
        this->sprite->setFlippedX(true);
    }
}

void BackPackBullet::onUpdate(float dt) {

}
