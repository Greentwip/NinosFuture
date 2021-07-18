#include "Door.h"
#include "./../GameTags.h"
#include "./../Armature.h"

#include "./../Input.h"

#include "./../AudioManager.h"

#include "./../AnimationAction.h"

#include "./../Sprite.h"


using namespace windy;

class DoorResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string DoorResources::spritePath = "sprites/gameplay/level/special/general/door/door";
std::string DoorResources::armaturePath = "physics/gameplay/level/special/general/door/door";


bool Door::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->lockTime = 1.0f;

    this->sprite = Sprite::create(DoorResources::spritePath);

    auto armature = Armature(DoorResources::armaturePath);

    auto newAnchor = armature.get("door").anchor;    
    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();
    this->sprite->setPosition(cocos2d::Point(0, 0) + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));
    
    this->collisionRectangles = armature.get("door").collisionRectangles;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];

    this->setTag(GameTags::General::Door);


    auto action = AnimationAction("lock", "door_lock", false, this->lockTime / 8.0f);

    this->sprite->appendAction(action, false);
    this->sprite->setAnimation("door_lock");
    this->sprite->setImageIndex(3);

    this->triggered = false;

    return true;
}

void Door::reopen() {
    this->sprite->setAnimation("door_lock");
    this->sprite->setImageIndex(3);
    this->setTag(GameTags::General::Door);
    this->triggered = false;

}

void Door::lock(std::function<void()> callback) {
    if (this->isBossDoor) { // Clearing input
        for (int i = 0; i < Input::joyKeys.size(); ++i) {
            Input::keys[Input::joyKeys[i]].status = KeyStatus::Up;
        }
    }

    auto delay = cocos2d::DelayTime::create(this->lockTime * 0.5f);

    auto lock = cocos2d::CallFunc::create([this]() {
        AudioManager::playSfx(Sounds::Door);
        this->sprite->runAction("lock");
        });

    auto postLock = cocos2d::CallFunc::create([=]() {
        callback();
        });

    auto sequence = cocos2d::Sequence::create(lock, delay, postLock, nullptr);

    this->stopAllActions();
    this->runAction(sequence);
    this->setTag(GameTags::General::Block);
}

void Door::unlock(std::function<void()> callback) {


    if (this->isBossDoor) { // Boss will handle joypad restore
        Input::takeInputs = false;
    }

    auto delay = cocos2d::DelayTime::create(this->lockTime * 0.5f);

    auto lock = cocos2d::CallFunc::create([this]() {
        AudioManager::playSfx(Sounds::Door);
        this->sprite->runAction("lock");
        this->sprite->reverseAction();
        });

    auto postLock = cocos2d::CallFunc::create([=]() {
        callback();
        });

    auto sequence = cocos2d::Sequence::create(lock, delay, postLock, nullptr);

    this->stopAllActions();
    this->runAction(sequence);
    this->triggered = true;

}



void Door::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Door::onUpdate(float dt) {

}