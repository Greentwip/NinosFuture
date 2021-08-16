
#include "IntroScene.h"
#include "GameStateMachine.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"
#include "Windy/AnimationAction.h"
#include "Windy/AudioManager.h"

#include "GameManager.h"

using namespace game;

cocos2d::Scene* IntroScene::scene()
{
    // 'scene' is an autorelease object
    cocos2d::Scene* scene = cocos2d::Scene::create();

    // 'layer' is an autorelease object
    IntroScene* layer = IntroScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool IntroScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();


    setCascadeOpacityEnabled(true);

    this->greentwipIntro();

    return true;
}

void IntroScene::greentwipIntro() {
    greentwipLogo =
        windy::Sprite::create("sprites/gameplay/screens/opening/greentwip/greentwip_logo", cocos2d::Point(0, 0));

    greentwipLogo->setPosition(0, 0);
    addChild(greentwipLogo);

    auto action = windy::AnimationAction("greentwip_logo", "greentwip_logo", false, 0.20f);

    greentwipLogo->appendAction(action, false);

    auto fadeIn = cocos2d::FadeIn::create(1.0f);

    auto preCallback = cocos2d::CallFunc::create([=]() {
        greentwipLogo->runAction("greentwip_logo");
        windy::AudioManager::playSfx(windy::Sounds::Intro);
        });

    auto duration = cocos2d::DelayTime::create(greentwipLogo->getActionDuration("greentwip_logo"));

    auto fadeOut = cocos2d::FadeOut::create(1.0f);

    auto postCallback = cocos2d::CallFunc::create([=]() {
        GameStateMachine::getInstance().pushState(GameState::Abakura);
//        auto levels = GameManager::getInstance().levels.collection;
//
//        for (auto level : levels) {
//            if (level->mug == "sheriffman") {
//                GameManager::getInstance().currentLevel = level;
//                break;
//            }
//        }

//        GameStateMachine::getInstance().pushState(GameState::Game);

        });

    auto sequence = cocos2d::Sequence::create(fadeIn, preCallback, duration, fadeOut, postCallback, nullptr);

    this->runAction(sequence);

}




void IntroScene::onEnter()
{
    Layer::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void IntroScene::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void IntroScene::update(float dt)
{
}
