
#include "BossIntroScene.h"
#include "GameStateMachine.h"

#include "Windy/Label.h"
#include "Windy/Armature.h"
#include "Windy/Sprite.h"
#include "Windy/AudioManager.h"
#include "Windy/Display.h"
#include "Windy/AnimationAction.h"
#include "Windy/LabelAction.h"

#include "GameManager.h"

#include "Intro.h"

using namespace game;

class BossIntroSceneResources {
public:
    static std::string bossPath;
    static std::string fontPath;

};

std::string BossIntroSceneResources::bossPath = "sprites/gameplay/screens/boss_intro/boss_animation/boss_animation";
std::string BossIntroSceneResources::fontPath = "fonts/megaman_2.ttf";

cocos2d::Scene* BossIntroScene::scene()
{
    // 'scene' is an autorelease object
    cocos2d::Scene* scene = cocos2d::Scene::create();

    // 'layer' is an autorelease object
    BossIntroScene* layer = BossIntroScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool BossIntroScene::init()
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

    auto fadeIn = cocos2d::FadeIn::create(1.0f);

    this->runAction(fadeIn);


    this->boss = windy::Sprite::create(BossIntroSceneResources::bossPath, cocos2d::Point(0.5f, 0));
    this->boss->setPosition(windy::Display::getInstance().center);
    this->boss->setVisible(false);
    this->addChild(this->boss, 256);

    this->boss->setPositionY(this->boss->getPositionY() - 16);

    auto action = windy::AnimationAction("animate", "boss_animation_" + GameManager::getInstance().currentLevel->mug, false, 0.10f);

    this->boss->appendAction(action, false);

    std::string mugText = GameManager::getInstance().currentLevel->mug;

    mugText.replace(mugText.find("man"), sizeof("man"), " man");

    auto labelAction = std::make_shared<windy::LabelAction>(0.2f, nullptr);

    this->text = 
        windy::Label::create(mugText, 
                             BossIntroSceneResources::fontPath, 
                             8,
                             cocos2d::TextHAlignment::CENTER,
                             cocos2d::TextVAlignment::TOP,
                             cocos2d::Point(0.5f, 0.5f),
                             labelAction);

    this->text->setPosition(windy::Display::getInstance().center);

    this->addChild(this->text, 256);

    this->text->setPositionY(this->text->getPositionY() - this->text->getContentSize().height * 0.5f - this->boss->getContentSize().height * 0.5f);

    this->intro = 
        Intro::create(GameManager::getInstance().currentLevel->mug, 
                      [this]() { this->onIntroComplete(); });

    this->addChild(this->intro);

    this->intro->setPosition(windy::Display::getInstance().left_bottom);

    this->triggered = false;

    this->bgmId = windy::AudioManager::playBgm(windy::Sounds::BossIntro, false);

    auto audioCallback = cocos2d::CallFunc::create([=]()
        {
            if (windy::AudioManager::getState(this->bgmId) == cocos2d::experimental::AudioEngine::AudioState::ERROR && !this->triggered)
            {
                windy::AudioManager::stopAll();
            }
        });


    auto audioDelay = cocos2d::DelayTime::create(10);

    auto sequence = cocos2d::Sequence::create(audioDelay, audioCallback, nullptr);

    this->runAction(sequence);


    return true;
}

void BossIntroScene::onEnter()
{
    Layer::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void BossIntroScene::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void BossIntroScene::onIntroComplete() {

    auto bossAnimate = cocos2d::CallFunc::create([this]() {
        this->boss->setVisible(true);
        this->boss->runAction("animate");
        });

    auto delay = cocos2d::DelayTime::create(this->boss->getActionDuration("animate"));

    auto textAnimation = cocos2d::CallFunc::create([this]() {
        this->onBossIntroComplete();
        });

    auto sequence = cocos2d::Sequence::create(bossAnimate, delay, textAnimation, nullptr);

    this->runAction(sequence);
}

void BossIntroScene::onBossIntroComplete() {
    this->text->startAnimation();
}


void BossIntroScene::update(float dt)
{

    if (windy::AudioManager::getState(this->bgmId) == cocos2d::experimental::AudioEngine::AudioState::ERROR && !this->triggered)
    {
        this->triggered = true;
        windy::AudioManager::stopAll();

        GameStateMachine::getInstance().pushState(GameState::Game);
    }
}
