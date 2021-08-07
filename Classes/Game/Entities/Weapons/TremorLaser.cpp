#include "TremorLaser.h"

#include "Windy/Sprite.h"
#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"

using namespace game;

class TremorLaserResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string TremorLaserResources::spritePath = "sprites/gameplay/level/weapon/tremor_laser/tremor_laser";
std::string TremorLaserResources::armaturePath = "physics/gameplay/level/weapon/tremor_laser/tremor_laser";


void TremorLaser::preloadResources() {
    windy::Armature::cache(TremorLaserResources::armaturePath);
    windy::Sprite::cache(TremorLaserResources::spritePath);
}

TremorLaser* TremorLaser::create() {
    TremorLaser* tremorLaser = new (std::nothrow) TremorLaser();

    if (tremorLaser && tremorLaser->init()) {
        tremorLaser->autorelease();
        return tremorLaser;
    }

    CC_SAFE_DELETE(tremorLaser);
    return nullptr;

}

bool TremorLaser::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}

void TremorLaser::setup() {

    this->sprite = windy::Sprite::create(TremorLaserResources::spritePath);
    this->addChild(this->sprite);

    auto armature = windy::Armature(TremorLaserResources::armaturePath);

    auto newAnchor = armature.get("tremor_laser").anchor;
    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();
    this->sprite->setPosition(cocos2d::Point(0, 0) + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    Logical::setup(this->getPosition(), armature.get("tremor_laser").collisionRectangles[0]->size);

    this->collisionRectangles = armature.get("tremor_laser").collisionRectangles;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];


    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("charge",        "tremor_laser_charge",         true,   0.05f),
        windy::AnimationAction("shoot",         "tremor_laser_shot",          true,   0.10f)
    };

    
    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("tremor_laser_shot");

    this->sprite->setPositionX(0 + this->sprite->getContentSize().width * 0.25f);

    this->sprite->setAnimation("tremor_laser_charge");
    this->sprite->runAction("charge");

    
    this->power = 4;
    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> TremorLaser::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {

    auto armature = windy::Armature(TremorLaserResources::armaturePath);

    auto newAnchor = armature.get("tremor_laser").anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get("tremor_laser").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}

void TremorLaser::onFinished() {
    this->finishForever();
}

void TremorLaser::disable() {
    this->setTag(windy::GameTags::Weapon::WeaponNone);
}

void TremorLaser::fire(int power, int direction, windy::GameTags::Weapon tag) {

    this->disable();

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    this->sprite->setFlippedX(flipX);
    this->speedBackup = cocos2d::Point(static_cast<float>(5 * direction), 0);

    auto chargeDelay = cocos2d::DelayTime::create(1);

    auto weaponizeCallback = cocos2d::CallFunc::create([=]() { 
        this->speed = this->speedBackup; 
        this->setTag(tag); 
    });
    
    auto animationCallback = cocos2d::CallFunc::create([this]() { 
        this->sprite->setAnimation("tremor_laser_shot");

        this->sprite->setPositionX(0 - this->sprite->getContentSize().width * 0.15f);

        this->sprite->runAction("shoot"); 
    });

    auto sequence = cocos2d::Sequence::create(chargeDelay, weaponizeCallback, animationCallback, nullptr);

    this->runAction(sequence);

}

void TremorLaser::onUpdate(float dt) {

}