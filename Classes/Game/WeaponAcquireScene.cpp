
#include "WeaponAcquireScene.h"
#include "GameStateMachine.h"

using namespace game;
using namespace cocos2d;

Scene* WeaponAcquireScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    WeaponAcquireScene* layer = WeaponAcquireScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool WeaponAcquireScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }

    auto onFinishCcallback = [=]() -> void {
        GameStateMachine::getInstance().pushState(GameState::Abakura);
    };

    return true;
}


void WeaponAcquireScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void WeaponAcquireScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void WeaponAcquireScene::update(float dt)
{
}
