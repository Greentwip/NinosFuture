#include <cmath>

#include "DirectionalBullet.h"

#include "Windy/Sprite.h"
#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"

using namespace game;

class DirectionalBulletResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string DirectionalBulletResources::spritePath = "sprites/gameplay/level/weapon/directional_bullet/directional_bullet";
std::string DirectionalBulletResources::armaturePath = "physics/gameplay/level/weapon/directional_bullet/directional_bullet";

void DirectionalBullet::preloadResources() {
    windy::Armature::cache(DirectionalBulletResources::armaturePath);
    windy::Sprite::cache(DirectionalBulletResources::spritePath);
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

    Logical::composite<windy::Weapon>(this,
                                      DirectionalBulletResources::armaturePath,
                                      DirectionalBulletResources::spritePath,
                                      "directional_bullet");


    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);


    auto action = windy::AnimationAction("shoot", "directional_bullet", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> DirectionalBullet::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, DirectionalBulletResources::armaturePath, "directional_bullet");
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