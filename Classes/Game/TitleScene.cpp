
#include "TitleScene.h"
#include "GameStateMachine.h"

#include "Windy/Label.h"
#include "Windy/Armature.h"
#include "Windy/Sprite.h"
#include "Windy/Input.h"

#include "Windy/Display.h"

#include "Windy/AudioManager.h"

#include "Game/Entities/UI/Fader.h"


class TitleSceneResources {
public:
    static std::string titleSpriteSet;
    static std::string font;

    static cocos2d::Color4B textActiveColor;
    static cocos2d::Color4B textInactiveColor;
};

std::string TitleSceneResources::titleSpriteSet = "sprites/gameplay/screens/title_screen/title_screen";
std::string TitleSceneResources::font = "fonts/megaman_2.ttf";

cocos2d::Color4B TitleSceneResources::textActiveColor = cocos2d::Color4B(255, 255, 255, 255);
cocos2d::Color4B TitleSceneResources::textInactiveColor = cocos2d::Color4B(127, 127, 127, 200);

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
    if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif

    auto background = windy::Sprite::create(TitleSceneResources::titleSpriteSet, cocos2d::Point(0, 0));
    auto city = windy::Sprite::create(TitleSceneResources::titleSpriteSet, cocos2d::Point(0, 0));
    auto silhouette = windy::Sprite::create(TitleSceneResources::titleSpriteSet);
    auto logo = windy::Sprite::create(TitleSceneResources::titleSpriteSet);
    auto abakura = windy::Sprite::create(TitleSceneResources::titleSpriteSet, cocos2d::Point(0, 0));
    auto greentwip = windy::Sprite::create(TitleSceneResources::titleSpriteSet, cocos2d::Point(0, 0));

    background->setPosition(0, 0);
    addChild(background);

    city->setPosition(0, 0);
    addChild(city);

    silhouette->setPosition(windy::Display::getInstance().width / 2.0f, windy::Display::getInstance().height / 3.25f);
    addChild(silhouette);

    logo->setPosition(windy::Display::getInstance().width / 2.0f, (windy::Display::getInstance().height * 0.25f) * 3);
    addChild(logo);

    abakura->setPosition(0, 0);
    addChild(abakura);

    greentwip->setPosition((float)windy::Display::getInstance().width - greentwip->getContentSize().width * 0.6f, 0);
    addChild(greentwip);


    background->setAnimation("background");
    city->setAnimation("city");
    silhouette->setAnimation("silhouette");
    logo->setAnimation("logo");
    abakura->setAnimation("abakura");
    greentwip->setAnimation("greentwip");

    this->startText = windy::Label::create("START GAME", 
                                           TitleSceneResources::font, 
                                           8, 
                                           cocos2d::TextHAlignment::LEFT,
                                           cocos2d::TextVAlignment::TOP);

    this->optionsText = windy::Label::create("OPTIONS", 
                                           TitleSceneResources::font, 
                                           8, 
                                           cocos2d::TextHAlignment::LEFT,
                                           cocos2d::TextVAlignment::TOP);    

    this->_aboutText = windy::Label::create("ABOUT", 
                                           TitleSceneResources::font, 
                                           8, 
                                           cocos2d::TextHAlignment::LEFT,
                                           cocos2d::TextVAlignment::TOP);    

    this->_projectsText = windy::Label::create("PROJECTS", 
                                           TitleSceneResources::font, 
                                           8, 
                                           cocos2d::TextHAlignment::LEFT,
                                           cocos2d::TextVAlignment::TOP);    

    this->_creditsText = windy::Label::create("CREDITS", 
                                           TitleSceneResources::font, 
                                           8, 
                                           cocos2d::TextHAlignment::LEFT,
                                           cocos2d::TextVAlignment::TOP);    
    float startTextWidth = this->startText->getContentSize().width;
    float startTextHeight = this->startText->getContentSize().height;

    float optionsTextWidth = this->optionsText->getContentSize().width;
    float optionsTextHeight = this->optionsText->getContentSize().height;

    float aboutTextWidth = this->_aboutText->getContentSize().width;
    float aboutTextHeight = this->_aboutText->getContentSize().height;

    float projectsTextWidth = this->_projectsText->getContentSize().width;
    float projectsTextHeight = this->_projectsText->getContentSize().height;

    float creditsTextWidth = this->_creditsText->getContentSize().width;
    float creditsTextHeight = this->_creditsText->getContentSize().height;

    this->startText->setPosition(
            cocos2d::Point((windy::Display::getInstance().width * 0.5f) - (startTextWidth * 0.5f),
              (windy::Display::getInstance().height * 0.5f) + (startTextHeight * 2)));

    this->optionsText->setPosition(
        cocos2d::Point((windy::Display::getInstance().width * 0.5f) - (optionsTextWidth * 0.5f),
            (windy::Display::getInstance().height * 0.5f) + (optionsTextHeight * 0.5f)));

    this->_aboutText->setPosition(
        cocos2d::Point((windy::Display::getInstance().width * 0.5f) - (aboutTextWidth * 0.5f),
            (windy::Display::getInstance().height * 0.5f) - (aboutTextHeight * 0.5f) * 2));


    this->_projectsText->setPosition(
        cocos2d::Point((windy::Display::getInstance().width * 0.5f) - (projectsTextWidth * 0.5f),
            (windy::Display::getInstance().height * 0.5f) - (projectsTextHeight * 0.5f) * 6));


    this->_creditsText->setPosition(
        cocos2d::Point((windy::Display::getInstance().width * 0.5f) - (creditsTextWidth * 0.5f),
            (windy::Display::getInstance().height * 0.5f) - (creditsTextHeight * 0.5f) * 8));

    addChild(this->startText, 128);
    addChild(this->optionsText, 128);
    addChild(this->_aboutText, 128);
    addChild(this->_projectsText, 128);
    addChild(this->_creditsText, 128);

    this->selectedOption = this->startText;

    selectStart();

    this->triggered = false;

    _ready = false;

    auto fader = Fader::create(cocos2d::Point(0, 0));

    fader->setPosition(cocos2d::Point(0, 0));

    fader->setOpacity(255);

    addChild(fader, 4096);

    _fader = fader;

    _fader->fadeOut([this]() {
        _ready = true;

        if (windy::AudioManager::getCurrentTrack() != windy::Sounds::Title) {
            windy::AudioManager::stopAll();
            windy::AudioManager::playBgm(windy::Sounds::Title);
        }

    });

    return true;
}

void TitleScene::selectStart() {
    this->startText->setTextColor(TitleSceneResources::textActiveColor);
    this->optionsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_aboutText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_projectsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_creditsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->selectedOption = this->startText;
}   

void TitleScene::selectOptions() {
    this->startText->setTextColor(TitleSceneResources::textInactiveColor);
    this->optionsText->setTextColor(TitleSceneResources::textActiveColor);
    this->_aboutText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_projectsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_creditsText->setTextColor(TitleSceneResources::textInactiveColor);

    this->selectedOption = this->optionsText;
}

void TitleScene::selectAbout() {
    this->startText->setTextColor(TitleSceneResources::textInactiveColor);
    this->optionsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_aboutText->setTextColor(TitleSceneResources::textActiveColor);
    this->_projectsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_creditsText->setTextColor(TitleSceneResources::textInactiveColor);

    this->selectedOption = this->_aboutText;
}



void TitleScene::selectProjects() {
    this->startText->setTextColor(TitleSceneResources::textInactiveColor);
    this->optionsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_aboutText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_projectsText->setTextColor(TitleSceneResources::textActiveColor);
    this->_creditsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->selectedOption = this->_projectsText;
}

void TitleScene::selectCredits() {
    this->startText->setTextColor(TitleSceneResources::textInactiveColor);
    this->optionsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_aboutText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_projectsText->setTextColor(TitleSceneResources::textInactiveColor);
    this->_creditsText->setTextColor(TitleSceneResources::textActiveColor);

    this->selectedOption = this->_creditsText;
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
    if (!this->triggered && _ready) {

        if (windy::Input::keyPressed(windy::InputKey::Down)) {
            if (this->selectedOption == this->startText) {
                this->selectOptions();
            }
            else if (this->selectedOption == this->optionsText) {
                this->selectAbout();
            }
            else if (this->selectedOption == this->_aboutText) {
                this->selectProjects();
            }
            else if (this->selectedOption == this->_projectsText) {
                this->selectCredits();
            }
            else if (this->selectedOption == this->_creditsText) {
                this->selectStart();
            }


            windy::AudioManager::playSfx(windy::Sounds::Select);
        }
        else if (windy::Input::keyPressed(windy::InputKey::Up)) {
            if (this->selectedOption == this->startText) {
                this->selectCredits();
            }
            else if (this->selectedOption == this->optionsText) {
                this->selectStart();
            }
            else if (this->selectedOption == this->_aboutText) {
                this->selectOptions();
            }
            else if (this->selectedOption == this->_projectsText) {
                this->selectAbout();
            }
            else if (this->selectedOption == this->_creditsText) {
                this->selectProjects();
            }


            windy::AudioManager::playSfx(windy::Sounds::Select);
        }

        if (windy::Input::keyPressed(windy::InputKey::A)) {
            this->triggered = true;
            windy::AudioManager::playSfx(windy::Sounds::Selected);

            _fader->fadeIn([this]() {
                
                if (this->selectedOption == this->startText) {
                    GameStateMachine::getInstance().pushState(GameState::Save);
                }
                else if (this->selectedOption == this->optionsText) {
                    GameStateMachine::getInstance().pushState(GameState::Options);
                }
                else if (this->selectedOption == this->_aboutText) {
                    GameStateMachine::getInstance().pushState(GameState::About);
                }
                else if (this->selectedOption == this->_projectsText) {
                    GameStateMachine::getInstance().pushState(GameState::Projects);

                }
                else if (this->selectedOption == this->_creditsText) {
                    GameStateMachine::getInstance().pushState(GameState::Credits);
                }

                                
            });

            

        }

    }
}
