
#include "TitleScene.h"
#include "GameStateMachine.h"

using namespace game;
using namespace cocos2d;

Scene* TitleScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    TitleScene* layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool TitleScene::init()
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


void TitleScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void TitleScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void TitleScene::update(float dt)
{
}
