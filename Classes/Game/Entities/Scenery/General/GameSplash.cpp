#include "GameSplash.h"

#include "Windy/AnimationAction.h"

#include "Windy/Sprite.h"

#include "Windy/GameTags.h"

using namespace game;

game::Resources& GameSplash::getResources() {
    static game::Resources resources{ game::ResourceKind::LevelSpecial, "splash", false };
    return resources;
}

GameSplash* GameSplash::create() {
    GameSplash* entity = new (std::nothrow) GameSplash();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;

}

bool GameSplash::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(windy::GameTags::Scenery::Particle);

    sprite = windy::Sprite::create(getResources()._spritePath, cocos2d::Point(0.5f, 0.5f));

    sprite->setPosition(0, 0);

    addChild(sprite);

    auto none = windy::AnimationAction("none", "splash_none", false, 0.10f);
    auto play = windy::AnimationAction("play", "splash_play", false, 0.10f);

    this->sprite->appendAction(none, false);
    this->sprite->appendAction(play, false);

    this->sprite->runAction("none");
    this->sprite->setAnimation("splash_none");

    return true;
}

void GameSplash::triggerSplash() {
    
    auto playAnimationCallback = cocos2d::CallFunc::create([this]() {
        //this->stopAllActions();
        this->sprite->stopActions();
        this->sprite->runAction("play");
    });

    auto delay = cocos2d::DelayTime::create(this->sprite->getActionDuration("play"));

    auto onEnd = cocos2d::CallFunc::create([this]() {
        this->sprite->stopActions();
        this->sprite->runAction("none"); 
        this->sprite->setAnimation("splash_none");
    });

    auto sequence = cocos2d::Sequence::create(playAnimationCallback, delay, onEnd, nullptr);

    this->runAction(sequence);

}