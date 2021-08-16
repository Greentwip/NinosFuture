
#include "GameOverScene.h"
#include "GameStateMachine.h"
#include "GameManager.h"

#include "cocos/ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "Windy/AudioManager.h"
#include "Windy/Input.h"
#include "Windy/SaveManager.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"



using namespace game;
using namespace cocos2d;

class GameOverSceneResources {
public:
    static std::string dataFile;
    static cocos2d::Color3B activeColor;
    static cocos2d::Color3B inactiveColor;

};

std::string GameOverSceneResources::dataFile = "sprites/gameplay/screens/gameover/data.csb";
cocos2d::Color3B GameOverSceneResources::activeColor = cocos2d::Color3B(255, 255, 255);
cocos2d::Color3B GameOverSceneResources::inactiveColor = cocos2d::Color3B(127, 127, 127);


Scene* GameOverScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    GameOverScene* layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    setCascadeOpacityEnabled(true);


    auto fadeIn = FadeIn::create(1.0f);

    runAction(fadeIn);


    auto root = cocos2d::CSLoader::createNode(GameOverSceneResources::dataFile);

    addChild(root);

    

    _yesButton = root->getChildByName("yes_button");
    _noButton = root->getChildByName("no_button");

    root->getChildByName("game_over_background")->setVisible(true);
    root->getChildByName("level_clear_background")->setVisible(false);

    _currentOption = _yesButton;

    _triggered = false;
    _ready = false;

    auto rootTimeline = cocos2d::CSLoader::createTimeline(GameOverSceneResources::dataFile);

    root->runAction(rootTimeline);

    rootTimeline->gotoFrameAndPause(0);

    auto flyPrimaryDelay = cocos2d::DelayTime::create((1.0f / 60.0f) * 30);
    auto flySecondaryDelay = cocos2d::DelayTime::create((1.0f / 60.0f) * 45);
    
    auto preFlyCallback = cocos2d::CallFunc::create([rootTimeline]() {
        rootTimeline->play("fly_in_primary", false);
    });

    auto flyInPrimaryCallback = cocos2d::CallFunc::create([rootTimeline]() {
        rootTimeline->play("fly_out_primary", false);
    });

    auto flyOutPrimaryCallback = cocos2d::CallFunc::create([rootTimeline]() {
        rootTimeline->play("fly_in_secondary", false);
    });

    auto flyInSecondaryCallback = cocos2d::CallFunc::create([this]() {
        _yesButton->setVisible(true);
        _noButton->setVisible(true);
        _ready = true;

        _yesButton->setColor(GameOverSceneResources::activeColor);
        _noButton->setColor(GameOverSceneResources::inactiveColor);
    });

    auto flyCallbacks = cocos2d::Sequence::create(preFlyCallback,
        flyPrimaryDelay,
        flyInPrimaryCallback,
        flyPrimaryDelay,
        flyOutPrimaryCallback,
        flySecondaryDelay,
        flyInSecondaryCallback,
        nullptr);

    auto flySequence = cocos2d::Sequence::create(
        flyPrimaryDelay,
        flyPrimaryDelay,
        flyPrimaryDelay,
        flyPrimaryDelay,
        flyCallbacks,
        nullptr
    );

    runAction(flySequence);


    windy::AudioManager::playSfx(windy::Sounds::LevelComplete);

    return true;
}


void GameOverScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void GameOverScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void GameOverScene::update(float dt)
{
    if (!_triggered && _ready) {
        
        if (windy::Input::keyPressed(windy::InputKey::Left) || windy::Input::keyPressed(windy::InputKey::Right)) {
            if (_currentOption == _yesButton) {
                _yesButton->setColor(GameOverSceneResources::inactiveColor);
                _noButton->setColor(GameOverSceneResources::activeColor);
                _currentOption = _noButton;
            }
            else {
                _yesButton->setColor(GameOverSceneResources::activeColor);
                _noButton->setColor(GameOverSceneResources::inactiveColor);
                _currentOption = _yesButton;
            }

            windy::AudioManager::playSfx(windy::Sounds::Select);
        }

        if (windy::Input::keyPressed(windy::InputKey::A)) {

            _triggered = true;

            windy::AudioManager::playSfx(windy::Sounds::Selected);

            GameManager::getInstance().player.lives = 3;

            auto slot = GameManager::getInstance().getDefaultSlot();

            slot.lives = GameManager::getInstance().player.lives;

            windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

            if (_currentOption == _yesButton) {
                GameStateMachine::getInstance().pushState(GameState::Game);
            }
            else {
                GameStateMachine::getInstance().pushState(GameState::StageSelect);
            }
                        
        }

    }
}
