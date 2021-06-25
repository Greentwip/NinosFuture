
#include "AbakuraScene.h"
#include "GameStateMachine.h"

#include "Windy/Sprite.h"
#include "Windy/AnimationAction.h"
#include "Windy/AudioManager.h"

using namespace game;

cocos2d::Scene* AbakuraScene::scene()
{
    // 'scene' is an autorelease object
    cocos2d::Scene* scene = cocos2d::Scene::create();

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
    if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    setCascadeOpacityEnabled(true);

    this->abakuraIntro();

    return true;
}

void AbakuraScene::abakuraIntro() {
    abakuraLogo =
        windy::Sprite::create("sprites/gameplay/screens/opening/abakura/abakura_logo", cocos2d::Point(0, 0));

    abakuraLogo->setPosition(0, 0);
    addChild(abakuraLogo);

    auto action = windy::AnimationAction("abakura_logo", "abakura_logo", false, 0.20f);

    abakuraLogo->appendAction(action, false);

    auto fadeIn = cocos2d::FadeIn::create(1.0f);

    auto preCallback = cocos2d::CallFunc::create([=]() {
        abakuraLogo->runAction("abakura_logo");
        windy::AudioManager::playSFX(windy::Sounds::Abakura);
        });

    auto duration = cocos2d::DelayTime::create(abakuraLogo->getActionDuration("abakura_logo"));

    auto durationAfter = cocos2d::DelayTime::create(abakuraLogo->getActionDuration("abakura_logo"));

    auto fadeOut = cocos2d::FadeOut::create(1.0f);

    auto postCallback = cocos2d::CallFunc::create([=]() {
        GameStateMachine::getInstance().pushState(GameState::Title);
        });

    auto sequence = cocos2d::Sequence::create(fadeIn, preCallback, duration, durationAfter, fadeOut, postCallback, nullptr);

    this->runAction(sequence);

}


void AbakuraScene::onEnter()
{
    Layer::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void AbakuraScene::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void AbakuraScene::update(float dt)
{
}
