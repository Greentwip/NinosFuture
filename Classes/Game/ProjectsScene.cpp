
#include "ProjectsScene.h"
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

class ProjectsSceneResources {
public:
    static std::string dataFile;
};

std::string ProjectsSceneResources::dataFile = "sprites/gameplay/screens/projects/data.csb";

Scene* ProjectsScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    ProjectsScene* layer = ProjectsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool ProjectsScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    auto root = cocos2d::CSLoader::createNode(ProjectsSceneResources::dataFile);

    addChild(root);

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

void ProjectsScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void ProjectsScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void ProjectsScene::update(float dt)
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
