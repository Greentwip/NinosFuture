//
// Created by castral on 8/17/21.
//

#include "TankBullet.h"

#include "Windy/GameTags.h"

using namespace game;

game::Resources& TankBullet::getResources() {
    static game::Resources resources{game::ResourceKind::Weapon, "tank_bullet"};
    return resources;
}

TankBullet* TankBullet::create() {
    auto* tankBullet = new (std::nothrow) TankBullet();

    if (tankBullet && tankBullet->init()) {
        tankBullet->autorelease();
        return tankBullet;
    }

    CC_SAFE_DELETE(tankBullet);
    return nullptr;
}

std::shared_ptr<cocos2d::Rect> TankBullet::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<TankBullet>(position, size);
}

bool TankBullet::init() {
    if (!Node::init()) {
        return false;
    }

    return true;
}

void TankBullet::setup() {
    power = 4;
    Logical::composite<TankBullet>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "tank_bullet", true, 0.10);
    sprite->appendAction(action, false);
    sprite->runAction("shoot");

    ignoreLandscapeCollision = true;
    setTag(windy::GameTags::WeaponEnemy);
}

void TankBullet::onFinished() {
    finishForever();
}

void TankBullet::fire(bool flipped) {
    if (!flipped) {
        speed.x = -_moveSpeed;
    } else {
        speed.x = _moveSpeed;
    }

    if (!_rising) {
        speed.y = _riseSpeed;
        _rising = true;
    }
}

void TankBullet::onUpdate(float dt) {

}
