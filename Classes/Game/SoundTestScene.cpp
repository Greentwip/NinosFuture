
#include "SoundTestScene.h"
#include "GameStateMachine.h"

#include "Windy/Input.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"

#include "GameStateMachine.h"

#include "Game/Entities/UI/Fader.h"

#include "Game/Entities/UI/Sound/SoundTestMenu.h"


using namespace game;
using namespace cocos2d;

Scene* SoundTestScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    SoundTestScene* layer = SoundTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool SoundTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    auto soundTestNode = SoundTestMenu::create();

    addChild(soundTestNode);


    this->triggered = false;

    _ready = false;

    auto fader = Fader::create(cocos2d::Point(0, 0));

    fader->setPosition(cocos2d::Point(0, 0));

    fader->setOpacity(255);

    addChild(fader, 4096);

    _fader = fader;

    _fader->fadeOut([this]() {
        _ready = true;
    });


    return true;
}
 
void SoundTestScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void SoundTestScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void SoundTestScene::update(float dt)
{

    if (!this->triggered) {
        if (windy::Input::keyPressed(windy::InputKey::B)) {
            this->triggered = true;

            _fader->fadeIn([this]() {
                GameStateMachine::getInstance().pushState(GameState::Title);
                });
        }
    }
}
