
#include "IntroScene.h"
#include "GameStateMachine.h"

#include "Game/Entities/UI/Fader.h"

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


    //setCascadeOpacityEnabled(true);

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

    auto fader = Fader::create(cocos2d::Point(0, 0));

    fader->setPosition(cocos2d::Point(0, 0));

    fader->setOpacity(255);

    addChild(fader, 4096);


    fader->fadeOut([=]() {
        auto preCallback = cocos2d::CallFunc::create([=]() {
            greentwipLogo->runAction("greentwip_logo");
            windy::AudioManager::playSfx(windy::Sounds::Intro);
            });

        auto duration = cocos2d::DelayTime::create(greentwipLogo->getActionDuration("greentwip_logo"));

        auto fadeOut = cocos2d::CallFunc::create([=]() {
            fader->fadeIn([=]() {
                GameStateMachine::getInstance().pushState(GameState::Abakura);
                /*auto levels = GameManager::getInstance().levels.collection;

                for (auto level : levels) {
                    if (level->mug == "sheriffman") {
                        GameManager::getInstance().currentLevel = level;
                        break;
                    }
                }

                GameStateMachine::getInstance().pushState(GameState::GetWeapon);*/

            });
        });

        auto sequence = cocos2d::Sequence::create(preCallback, duration, fadeOut, nullptr);

        this->runAction(sequence);

    });
    
}