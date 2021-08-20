#include <string>
#include <map>

#include "StageSelectScene.h"
#include "GameStateMachine.h"

#include "GameManager.h"

#include "cocos/ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"

#include "Windy/AudioManager.h"

#include "Windy/Input.h"

#include "Game/Entities/UI/Fader.h"


using namespace game;
using namespace cocos2d;

class StageSelectSceneResources {
public:
    static std::string dataFile;
    static std::string cursorSpriteSet;
    static std::map<std::string, std::map<std::string, cocos2d::Node*>> mugLocations;
};

std::string StageSelectSceneResources::dataFile = "sprites/gameplay/screens/stage_select/data.csb";
std::string StageSelectSceneResources::cursorSpriteSet = "sprites/gameplay/level/ui/selector/selector";
std::map<std::string, std::map<std::string, cocos2d::Node*>> StageSelectSceneResources::mugLocations;

Scene* StageSelectScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    StageSelectScene* layer = StageSelectScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool StageSelectScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    auto root = cocos2d::CSLoader::createNode(StageSelectSceneResources::dataFile);

    addChild(root);

    this->sheriffMan = root->getChildByName("sheriffman");
    this->militaryMan = root->getChildByName("militaryman");
    this->vineMan = root->getChildByName("vineman");
    this->nightMan = root->getChildByName("nightman");
    this->swing = root->getChildByName("swing");


    this->cursor = windy::Sprite::create(StageSelectSceneResources::cursorSpriteSet, cocos2d::Point(0.5, 0.5));

    this->cursor->setPosition(swing->getPosition());

    addChild(this->cursor);

    this->cursor->setAnimation("square_small");

    auto blink = Blink::create(0.5, 4);
    auto sequence = RepeatForever::create(blink);

    dynamic_cast<Node*>(this->cursor)->runAction(sequence);


    this->positionX = "middle";
    this->positionY = "middle";

    this->selectedMug = swing;

    StageSelectSceneResources::mugLocations["middle"]["middle"] = this->swing;
    StageSelectSceneResources::mugLocations["middle"]["top"] = this->sheriffMan;
    StageSelectSceneResources::mugLocations["left"]["middle"] = this->militaryMan;
    StageSelectSceneResources::mugLocations["right"]["middle"] = this->vineMan;
    StageSelectSceneResources::mugLocations["middle"]["bottom"] = this->nightMan;

    this->triggered = false;

    _ready = false;

    windy::AudioManager::stopAll();
    windy::AudioManager::playBgm(windy::Sounds::StageSelect);

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


void StageSelectScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void StageSelectScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}


void StageSelectScene::moveLeft()
{
    bool playFx = true;
    bool move = true;

    if (this->positionY == "top" || this->positionY == "bottom") {
        playFx = false;
        move = false;
    }
    
    if (move) {
        if (this->positionX == "middle") {
            this->positionX = "left";
        }
        else if (this->positionX == "left") {
            this->positionX = "right";
        }
        else if (this->positionX == "right") {
            this->positionX = "middle";
        }
    }

    if (playFx) {
        windy::AudioManager::playSfx(windy::Sounds::Select);
    }

}

void StageSelectScene::moveRight()
{
    bool playFx = true;
    bool move = true;

    if (this->positionY == "top" || this->positionY == "bottom") {
        playFx = false;
        move = false;
    }

    if (move) {
        if (this->positionX == "middle") {
            this->positionX = "right";
        }
        else if (this->positionX == "right") {
            this->positionX = "left";
        }
        else if (this->positionX == "left") {
            this->positionX = "middle";
        }
    }

    if (playFx) {
        windy::AudioManager::playSfx(windy::Sounds::Select);
    }

}

void StageSelectScene::moveUp()
{
    bool playFx = true;
    bool move = true;

    if (this->positionX == "left" || this->positionX == "right") {
        playFx = false;
        move = false;
    }

    if (move) {
        if (this->positionY == "middle") {
            this->positionY = "top";
        }
        else if (this->positionY == "top") {
            this->positionY = "bottom";
        }
        else if (this->positionY == "bottom") {
            this->positionY = "middle";
        }
    }

    if (playFx) {
        windy::AudioManager::playSfx(windy::Sounds::Select);
    }

}

void StageSelectScene::moveDown()
{
    bool playFx = true;
    bool move = true;

    if (this->positionX == "left" || this->positionX == "right") {
        playFx = false;
        move = false;
    }

    if (move) {
        if (this->positionY == "middle") {
            this->positionY = "bottom";
        }
        else if (this->positionY == "bottom") {
            this->positionY = "top";
        }
        else if (this->positionY == "top") {
            this->positionY = "middle";
        }
    }

    if (playFx) {
        windy::AudioManager::playSfx(windy::Sounds::Select);
    }
}

void StageSelectScene::update(float dt)
{
    if (!this->triggered && _ready) {
        if (windy::Input::keyPressed(windy::InputKey::Up)) {
            this->moveUp();
            this->cursor->setPosition(StageSelectSceneResources::mugLocations[this->positionX][this->positionY]->getPosition());
            this->selectedMug = StageSelectSceneResources::mugLocations[this->positionX][this->positionY];
        }
        else if (windy::Input::keyPressed(windy::InputKey::Down)) {
            this->moveDown();
            this->cursor->setPosition(StageSelectSceneResources::mugLocations[this->positionX][this->positionY]->getPosition());
            this->selectedMug = StageSelectSceneResources::mugLocations[this->positionX][this->positionY];
        }
        else if (windy::Input::keyPressed(windy::InputKey::Left)) {
            this->moveLeft();
            this->cursor->setPosition(StageSelectSceneResources::mugLocations[this->positionX][this->positionY]->getPosition());
            this->selectedMug = StageSelectSceneResources::mugLocations[this->positionX][this->positionY];
        }
        else if (windy::Input::keyPressed(windy::InputKey::Right)) {
            this->moveRight();
            this->cursor->setPosition(StageSelectSceneResources::mugLocations[this->positionX][this->positionY]->getPosition());
            this->selectedMug = StageSelectSceneResources::mugLocations[this->positionX][this->positionY];
        }

        if (windy::Input::keyPressed(windy::InputKey::B)) {
            this->triggered = true;

            _fader->fadeIn([this]() {
                GameStateMachine::getInstance().pushState(GameState::Save);
            });
        }
        else if (windy::Input::keyPressed(windy::InputKey::A) &&
            this->selectedMug != StageSelectSceneResources::mugLocations["middle"]["middle"]) {
            this->triggered = true;

            windy::AudioManager::playSfx(windy::Sounds::Selected);

            auto levels = GameManager::getInstance().levels.collection;

            for (auto level : levels) {
                if (level->mug == this->selectedMug->getName()) {
                    GameManager::getInstance().currentLevel = level;
                    break;
                }
            }

            _fader->fadeIn([this]() {
                GameStateMachine::getInstance().pushState(GameState::BossIntro);
            });

        }
    }

    



}
