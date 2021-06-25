
#include "AbakuraScene.h"
#include "GameStateMachine.h"

using namespace game;
using namespace cocos2d;

Scene* AbakuraScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    AbakuraScene* layer = AbakuraScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool AbakuraScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }

    auto onFinishCcallback = [=]() -> void {
        GameStateMachine::getInstance().pushState(GameState::Intro);
    };

    return true;
}


void AbakuraScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void AbakuraScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void AbakuraScene::update(float dt)
{
}
