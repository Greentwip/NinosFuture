#include "TremorTail.h"

#include "Windy/Level.h"
#include "Windy/ObjectManager.h"

#include "Game/Entities/Weapons/TremorDrill.h"

using namespace game;

TremorTail* TremorTail::create() {

    TremorTail* tremorTail = new (std::nothrow) TremorTail();

    if (tremorTail && tremorTail->init()) {
        tremorTail->autorelease();
        return tremorTail;
    }

    CC_SAFE_DELETE(tremorTail);
    return nullptr;
}

bool TremorTail::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }


    this->drill = TremorDrill::create();
    this->addChild(drill);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    /*auto entryCollisionBox = TremorDrill::getEntryCollisionRectangle(cocos2d::Point(0, 0), cocos2d::Size(16, 16));
    auto entry = windy::Logical::getEntry(entryCollisionBox, [=]() {
        auto weapon = TremorDrill::create();
        weapon->setPosition(cocos2d::Point(0, 0));
        return weapon;
    });

    this->level->objectManager->objectEntries.push_back(entry);*/

    return true;
}


void TremorTail::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void TremorTail::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void TremorTail::onFinished() {
    
}


void TremorTail::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void TremorTail::onUpdate(float dt) {

}