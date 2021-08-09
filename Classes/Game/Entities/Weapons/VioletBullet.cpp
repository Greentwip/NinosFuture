#include "VioletBullet.h"

#include "Windy/Sprite.h"
#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"

using namespace game;

class VioletBulletResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string VioletBulletResources::spritePath = "sprites/gameplay/level/weapon";
std::string VioletBulletResources::armaturePath = "physics/gameplay/level/weapon";

void VioletBullet::preloadResources() {
    {
        std::string bulletName = std::string("violet_bullet") + std::string("_") + "low";
        std::string bulletPath = bulletName + "/" + bulletName;

        windy::Armature::cache(VioletBulletResources::armaturePath + "/" + bulletPath);
        windy::Sprite::cache(VioletBulletResources::spritePath + "/" + bulletPath);
    }

    {
        std::string bulletName = std::string("violet_bullet") + std::string("_") + "mid";
        std::string bulletPath = bulletName + "/" + bulletName;

        windy::Armature::cache(VioletBulletResources::armaturePath + "/" + bulletPath);
        windy::Sprite::cache(VioletBulletResources::spritePath + "/" + bulletPath);
    }

    {
        std::string bulletName = std::string("violet_bullet") + std::string("_") + "high";
        std::string bulletPath = bulletName + "/" + bulletName;

        windy::Armature::cache(VioletBulletResources::armaturePath + "/" + bulletPath);
        windy::Sprite::cache(VioletBulletResources::spritePath + "/" + bulletPath);
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

void VioletBullet::setup(const std::string& powerLevel) {

    std::string bulletName = std::string("violet_bullet") + std::string("_") + powerLevel;
    std::string bulletPath = bulletName + "/" + bulletName;

    Logical::composite<windy::Weapon>(this,
                                      VioletBulletResources::armaturePath + "/" + bulletPath,
                                      VioletBulletResources::spritePath + "/" + bulletPath,
                                      bulletName);

    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);


    auto action = windy::AnimationAction("shoot", std::string("violet_bullet") + std::string("_") + powerLevel, true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> VioletBullet::getEntryCollisionRectangle(const std::string& powerLevel,
                                                                        const cocos2d::Point& position,
                                                                        const cocos2d::Size& size) {

    std::string bulletName = std::string("violet_bullet") + std::string("_") + powerLevel;
    std::string bulletPath = bulletName + "/" + bulletName;

    return Logical::buildEntryCollisionRectangle(position, size, VioletBulletResources::armaturePath + "/" + bulletPath, bulletName);
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