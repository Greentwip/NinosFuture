#include <cmath>

#include "ExtremeBullet.h"

#include "Windy/Sprite.h"
#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"

using namespace game;

class ExtremeBulletResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string ExtremeBulletResources::spritePath = "sprites/gameplay/level/weapon/extreme_bullet/extreme_bullet";
std::string ExtremeBulletResources::armaturePath = "physics/gameplay/level/weapon/extreme_bullet/extreme_bullet";

void ExtremeBullet::preloadResources() {
    windy::Armature::cache(ExtremeBulletResources::armaturePath);
    windy::Sprite::cache(ExtremeBulletResources::spritePath);
}


ExtremeBullet* ExtremeBullet::create() {
    ExtremeBullet* entity = new (std::nothrow) ExtremeBullet();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;

}

bool ExtremeBullet::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void ExtremeBullet::setup() {

    Logical::composite<windy::Weapon>(this, ExtremeBulletResources::armaturePath, ExtremeBulletResources::spritePath, "extreme_bullet");
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "extreme_bullet", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> ExtremeBullet::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, ExtremeBulletResources::armaturePath, "extreme_bullet");
}

void ExtremeBullet::onFinished() {
    this->finishForever();
}



void ExtremeBullet::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);

    float velocity = 4;

    this->speed = cocos2d::Point(static_cast<float>(velocity * direction), 0);

    this->setTag(tag);
}

void ExtremeBullet::onUpdate(float dt) {

}