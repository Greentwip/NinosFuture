#include "VioletBullet.h"

#include "./../Sprite.h"
#include "./../Armature.h"
#include "./../AnimationAction.h"

using namespace windy;

class VioletBulletResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string VioletBulletResources::spritePath = "sprites/gameplay/level/weapon";
std::string VioletBulletResources::armaturePath = "physics/gameplay/level/weapon";

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

    this->sprite = Sprite::create(VioletBulletResources::spritePath + "/" + bulletPath);
    this->addChild(this->sprite);

    auto armature = Armature(VioletBulletResources::armaturePath + "/" + bulletPath);

    auto newAnchor = armature.get(bulletName).anchor;
    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();
    this->sprite->setPosition(cocos2d::Point(0, 0) + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    Logical::setup(this->getPosition(), armature.get(bulletName).collisionRectangles[0]->size);

    this->collisionRectangles = armature.get(bulletName).collisionRectangles;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];
    auto action = AnimationAction("shoot", std::string("violet_bullet") + std::string("_") + powerLevel, true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

}

void VioletBullet::onUpdate(float dt) {

}