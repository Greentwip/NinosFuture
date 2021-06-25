
#include "OptionsScene.h"
#include "GameStateMachine.h"

using namespace game;
using namespace cocos2d;

Scene* OptionsScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    OptionsScene* layer = OptionsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool OptionsScene::init()
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


void OptionsScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void OptionsScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void OptionsScene::update(float dt)
{
}
