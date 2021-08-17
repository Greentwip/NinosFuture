#include "VioletBullet.h"

#include "Windy/Sprite.h"
#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"

using namespace game;
using ChargePower = windy::Browner::ChargePower;
using Resources = game::Resources;

static Resources& getLowResource() {
    static Resources resource {game::ResourceKind::Weapon, "violet_bullet_low"};
    return resource;
}

static Resources& getMidResource() {
    static Resources resource {game::ResourceKind::Weapon, "violet_bullet_mid"};
    return resource;
}

static Resources& getHighResource() {
    static Resources resource {game::ResourceKind::Weapon, "violet_bullet_high"};
    return resource;
}

Resources& VioletBullet::getResources(ChargePower chargePower) {
    switch (chargePower) {
        case ChargePower::low:
            return getLowResource();
        case ChargePower::mid:
            return getMidResource();
        case ChargePower::high:
            return getHighResource();
    }
}

VioletBullet* VioletBullet::create() {
    VioletBullet* violetBullet = new (std::nothrow) VioletBullet();

    if (violetBullet && violetBullet->init()) {
        violetBullet->autorelease();
        return violetBullet;
    }

    CC_SAFE_DELETE(violetBullet);
    return nullptr;

}

bool VioletBullet::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void VioletBullet::setup(ChargePower powerLevel) {
    const auto& resources = VioletBullet::getResources(powerLevel);
    Logical::composite<windy::Weapon>(this, resources);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);


    auto action = windy::AnimationAction("shoot", resources._entityName, true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> VioletBullet::getEntryCollisionRectangle(ChargePower powerLevel,
                                                                        const cocos2d::Point& position,
                                                                        const cocos2d::Size& size) {
    const auto& resources = VioletBullet::getResources(powerLevel);
    return Logical::buildEntryCollisionRectangle(resources, position, size);
}

void VioletBullet::onFinished() {
    this->finishForever();
}


void VioletBullet::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);
    this->speed = cocos2d::Point(static_cast<float>(6 * direction), 0);

    this->setTag(tag);
}


void VioletBullet::onUpdate(float dt) {

}
