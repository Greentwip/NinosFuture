
#include "CreditsScene.h"
#include "GameStateMachine.h"

#include "cocos/ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "Windy/Settings.h"
#include "Windy/Input.h"
#include "Windy/AudioManager.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"

#include "GameStateMachine.h"

#include "Game/Entities/UI/Fader.h"


using namespace game;
using namespace cocos2d;

class CreditsSceneSceneResources {
public:
    static std::string dataFile;
};

std::string CreditsSceneSceneResources::dataFile = "sprites/gameplay/screens/credits/data.csb";

Scene* CreditsScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    CreditsScene* layer = CreditsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool CreditsScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    auto root = cocos2d::CSLoader::createNode(CreditsSceneSceneResources::dataFile);

    addChild(root);

    auto rootTimeline = cocos2d::CSLoader::createTimeline(CreditsSceneSceneResources::dataFile);

    root->runAction(rootTimeline);

    rootTimeline->gotoFrameAndPause(0);


    _triggered = false;

    _ready = false;

    auto fader = Fader::create(cocos2d::Point(0, 0));

    fader->setPosition(cocos2d::Point(0, 0));

    fader->setOpacity(255);

    addChild(fader, 4096);

    _fader = fader;

    _fader->fadeOut([this, rootTimeline]() {
        
        auto flyPrimaryDelay = cocos2d::DelayTime::create((1.0f / 60.0f) * 1200);
        auto flyInPrimaryCallback = cocos2d::CallFunc::create([this, rootTimeline]() {
            _ready = true;
        });

        auto flySequence = cocos2d::Sequence::create(
            flyPrimaryDelay,
            flyInPrimaryCallback,
            nullptr
        );

        rootTimeline->play("scroll", false);

        runAction(flySequence);


    });


    return true;
}

void CreditsScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void CreditsScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void CreditsScene::update(float dt)
{

    if (!this->_triggered && _ready) {

        if (windy::Input::keyPressed(windy::InputKey::A)) {
            this->_triggered = true;

            windy::AudioManager::playSfx(windy::Sounds::Selected);

            _fader->fadeIn([this]() {
                GameStateMachine::getInstance().pushState(GameState::Title);
            });
        }
    }
}
