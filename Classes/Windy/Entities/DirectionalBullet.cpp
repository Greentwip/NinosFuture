#include <cmath>

#include "DirectionalBullet.h"

#include "./../Sprite.h"
#include "./../Armature.h"
#include "./../AnimationAction.h"

using namespace windy;

class DirectionalBulletResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string DirectionalBulletResources::spritePath = "sprites/gameplay/level/weapon/directional_bullet/directional_bullet";
std::string DirectionalBulletResources::armaturePath = "physics/gameplay/level/weapon/directional_bullet/directional_bullet";

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

    this->sprite = Sprite::create(DirectionalBulletResources::spritePath );
    this->addChild(this->sprite);

    auto armature = Armature(DirectionalBulletResources::armaturePath);

    auto newAnchor = armature.get("directional_bullet").anchor;
    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();
    this->sprite->setPosition(cocos2d::Point(0, 0) + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    Logical::setup(this->getPosition(), armature.get("directional_bullet").collisionRectangles[0]->size);

    this->collisionRectangles = armature.get("directional_bullet").collisionRectangles;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];
    auto action = AnimationAction("shoot", "directional_bullet", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->ignoreGravity = true;

    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> DirectionalBullet::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {

    auto armature = Armature(DirectionalBulletResources::armaturePath);

    auto newAnchor = armature.get("directional_bullet").anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get("directional_bullet").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}

void DirectionalBullet::onFinished() {
    this->finishForever();
}



void DirectionalBullet::fire(int power, int direction, GameTags::Weapon tag) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);
    this->speed = cocos2d::Point(static_cast<float>(4 * direction), 0);

    this->setTag(tag);
}



void DirectionalBullet::fire(int power, const cocos2d::Point& playerPosition, GameTags::Weapon tag) {

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