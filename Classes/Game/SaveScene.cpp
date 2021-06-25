
#include "SaveScene.h"
#include "GameStateMachine.h"

using namespace game;
using namespace cocos2d;

Scene* SaveScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    SaveScene* layer = SaveScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool SaveScene::init()
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


void SaveScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void SaveScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void SaveScene::update(float dt)
{
}
