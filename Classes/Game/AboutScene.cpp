
#include "AboutScene.h"
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

class AboutSceneResources {
public:
    static std::string dataFile;
    static cocos2d::Color3B activeColor;
    static cocos2d::Color3B inactiveColor;

};

std::string AboutSceneResources::dataFile = "sprites/gameplay/screens/about/data.csb";
cocos2d::Color3B AboutSceneResources::activeColor = cocos2d::Color3B(255, 255, 255);
cocos2d::Color3B AboutSceneResources::inactiveColor = cocos2d::Color3B(127, 127, 127);

Scene* AboutScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    AboutScene* layer = AboutScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool AboutScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    auto root = cocos2d::CSLoader::createNode(AboutSceneResources::dataFile);

    addChild(root);

    _root = root;

    _previousButton = root->getChildByName("prev_button");
    _nextButton = root->getChildByName("next_button");

    _currentOption = this->_nextButton;

    _previousButton->setColor(AboutSceneResources::inactiveColor);
    _nextButton->setColor(AboutSceneResources::activeColor);

    _menuIndex = 0;

    ((cocos2d::ui::Text*)_previousButton->getChildByName("label"))->setString("exit");

    for (int i = 0; i <= 8; ++i) {
        _root->getChildByName("about" + std::string("_") + std::to_string(i))->setVisible(false);
    }

    _root->getChildByName("about" + std::string("_") + std::to_string(_menuIndex))->setVisible(true);


    _triggered = false;

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

void AboutScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void AboutScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void AboutScene::update(float dt)
{

    if (!this->_triggered && _ready) {
        if (windy::Input::keyPressed(windy::InputKey::Left) || windy::Input::keyPressed(windy::InputKey::Right)) {
            if (this->_currentOption == this->_previousButton) {
                this->_previousButton->setColor(AboutSceneResources::inactiveColor);
                this->_nextButton->setColor(AboutSceneResources::activeColor);
                this->_currentOption = this->_nextButton;
            }
            else {
                this->_previousButton->setColor(AboutSceneResources::activeColor);
                this->_nextButton->setColor(AboutSceneResources::inactiveColor);
                this->_currentOption = this->_previousButton;
            }

            windy::AudioManager::playSfx(windy::Sounds::Select);

        }

        if (_menuIndex == 0) {
            ((cocos2d::ui::Text*)_previousButton->getChildByName("label"))->setString("exit");
            ((cocos2d::ui::Text*)_nextButton->getChildByName("label"))->setString("next");
        }
        else if (_menuIndex == 8) {
            ((cocos2d::ui::Text*)_previousButton->getChildByName("label"))->setString("prev");
            ((cocos2d::ui::Text*)_nextButton->getChildByName("label"))->setString("exit");
        }

        if (_menuIndex > 0 && _menuIndex < 8) {
            ((cocos2d::ui::Text*)_previousButton->getChildByName("label"))->setString("prev");
            ((cocos2d::ui::Text*)_nextButton->getChildByName("label"))->setString("next");
        }

        if ((_menuIndex == 8 && _currentOption == _nextButton) || (_menuIndex == 0 && _currentOption == _previousButton)) {
            if (windy::Input::keyPressed(windy::InputKey::A)) {
                this->_triggered = true;

                windy::AudioManager::playSfx(windy::Sounds::Selected);

                _fader->fadeIn([this]() {
                    GameStateMachine::getInstance().pushState(GameState::Title);
                });
            }
        }

        if (_currentOption == _previousButton && _menuIndex > 0) {
            if (windy::Input::keyPressed(windy::InputKey::A)) {
                _root->getChildByName("about" + std::string("_") + std::to_string(_menuIndex))->setVisible(false);
                _menuIndex -= 1;
                _root->getChildByName("about" + std::string("_") + std::to_string(_menuIndex))->setVisible(true);
            }
        }


        if (_currentOption == _nextButton && _menuIndex < 8) {
            if (windy::Input::keyPressed(windy::InputKey::A)) {
                _root->getChildByName("about" + std::string("_") + std::to_string(_menuIndex))->setVisible(false);
                _menuIndex += 1;
                _root->getChildByName("about" + std::string("_") + std::to_string(_menuIndex))->setVisible(true);
            }
        }
    }
}
