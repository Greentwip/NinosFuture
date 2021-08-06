#include "Door.h"
#include "./../GameTags.h"

#include "./../Input.h"

#include "./../AudioManager.h"

#include "./../Sprite.h"


using namespace windy;

bool Door::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->lockTime = 1.0f;

    this->setup();

    this->triggered = false;

    this->setTag(GameTags::General::Door);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    return true;
}


void Door::close() {
    this->setTag(GameTags::General::Block);
    this->triggered = false;
    //this->sprite->runAction("idle");

    this->sprite->runAction("lock");

}


void Door::open() {
    //this->sprite->setAnimation(this->prefix + "_" + "door_lock");
    //this->sprite->setImageIndex(3);
    this->sprite->runAction("lock");
    this->sprite->reverseAction();
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