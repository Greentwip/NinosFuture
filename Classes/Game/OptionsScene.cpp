
#include "OptionsScene.h"
#include "GameStateMachine.h"

#include "cocos/ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "Windy/Settings.h"
#include "Windy/Input.h"
#include "Windy/AudioManager.h"

#include "GameStateMachine.h"

using namespace game;
using namespace cocos2d;

class OptionsSceneResources {
public:
    static std::string dataFile;
    static cocos2d::Color3B activeColor;
    static cocos2d::Color3B inactiveColor;

};

std::string OptionsSceneResources::dataFile = "sprites/gameplay/screens/options/data.csb";
cocos2d::Color3B OptionsSceneResources::activeColor = cocos2d::Color3B(255, 255, 255);
cocos2d::Color3B OptionsSceneResources::inactiveColor = cocos2d::Color3B(127, 127, 127);

Scene* OptionsScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    OptionsScene* layer = OptionsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool OptionsScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }


    auto fadeIn = FadeIn::create(1.0f);

    this->runAction(fadeIn);

    auto root = cocos2d::CSLoader::createNode(OptionsSceneResources::dataFile);

    addChild(root);

    this->bgm = root->getChildByName("bgm");
    this->sfx = root->getChildByName("sfx");

    this->currentOption = this->bgm;

    this->bgmSlider = dynamic_cast<cocos2d::ui::Slider*>(this->bgm->getChildByName("slider"));
    this->sfxSlider = dynamic_cast<cocos2d::ui::Slider*>(this->sfx->getChildByName("slider"));

    this->bgmVolume = windy::Settings::bgmVolume * 100;
    this->sfxVolume = windy::Settings::sfxVolume * 100;

    this->bgmSlider->setPercent((int)this->bgmVolume);
    this->sfxSlider->setPercent((int)this->sfxVolume);

    this->triggered = false;

    return true;
}

void OptionsScene::save(float bgmVolume, float sfxVolume) {
    windy::Settings::bgmVolume = bgmVolume;
    windy::Settings::sfxVolume = sfxVolume;

    windy::Settings::apply();
}


void OptionsScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void OptionsScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void OptionsScene::update(float dt)
{

    if (!this->triggered) {
        if (windy::Input::keyPressed(windy::InputKey::Up) || windy::Input::keyPressed(windy::InputKey::Down)) {
            if (this->currentOption == this->bgm) {
                this->bgm->setColor(OptionsSceneResources::inactiveColor);                
                this->sfx->setColor(OptionsSceneResources::activeColor);
                this->currentOption = this->sfx;
            }
            else {
                this->bgm->setColor(OptionsSceneResources::activeColor);
                this->sfx->setColor(OptionsSceneResources::inactiveColor);
                this->currentOption = this->bgm;
            }
        }

        if (windy::Input::keyPressed(windy::InputKey::Left)) {
            if (this->currentOption == this->bgm) {
                if (this->bgmVolume > 0) {
                    this->bgmVolume -= 10;
                    this->bgmSlider->setPercent((int)this->bgmVolume);
                    this->save(this->bgmVolume / 100.0f, this->sfxVolume / 100.0f);
                }
            }
            else {
                if (this->sfxVolume > 0) {
                    this->sfxVolume -= 10;
                    this->sfxSlider->setPercent((int)this->sfxVolume);
                    this->save(this->bgmVolume / 100.0f, this->sfxVolume / 100.0f);
                    windy::AudioManager::playSfx(windy::Sounds::Select);
                }
            }
        }

        if (windy::Input::keyPressed(windy::InputKey::Right)) {
            if (this->currentOption == this->bgm) {
                if (this->bgmVolume < 100) {
                    this->bgmVolume += 10;
                    this->bgmSlider->setPercent((int)this->bgmVolume);
                    this->save(this->bgmVolume / 100.0f, this->sfxVolume / 100.0f);
                }
            }
            else {
                if (this->sfxVolume < 100) {
                    this->sfxVolume += 10;
                    this->sfxSlider->setPercent((int)this->sfxVolume);
                    this->save(this->bgmVolume / 100.0f, this->sfxVolume / 100.0f);
                    windy::AudioManager::playSfx(windy::Sounds::Select);
                }
            }
        }

    }

    if (!this->triggered) {
        if (windy::Input::keyPressed(windy::InputKey::B)) {
            this->triggered = true;
            windy::Settings::save();
            GameStateMachine::getInstance().pushState(GameState::Title);
        }
    }
}
