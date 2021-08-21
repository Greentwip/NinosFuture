
#include "MicrosoftScene.h"
#include "GameStateMachine.h"

#include "cocos/ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "Game/Entities/UI/Fader.h"


#include "Windy/Armature.h"
#include "Windy/Sprite.h"
#include "Windy/AnimationAction.h"
#include "Windy/AudioManager.h"

using namespace game;


class MicrosoftSceneResources {
public:
    static std::string dataFile;
};

std::string MicrosoftSceneResources::dataFile = "sprites/gameplay/screens/license/data.csb";

cocos2d::Scene* MicrosoftScene::scene()
{
    // 'scene' is an autorelease object
    cocos2d::Scene* scene = cocos2d::Scene::create();

    // 'layer' is an autorelease object
    MicrosoftScene* layer = MicrosoftScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool MicrosoftScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    this->intro();

    return true;
}

void MicrosoftScene::intro() {

    auto root = cocos2d::CSLoader::createNode(MicrosoftSceneResources::dataFile);

    addChild(root);

    auto fader = Fader::create(cocos2d::Point(0, 0));

    fader->setPosition(cocos2d::Point(0, 0));

    fader->setOpacity(255);

    addChild(fader, 4096);


    fader->fadeOut([=]() {

        auto duration = cocos2d::DelayTime::create(3);

        auto fadeOut = cocos2d::CallFunc::create([=]() {
            fader->fadeIn([=]() {
                GameStateMachine::getInstance().pushState(GameState::Title);
                });
            });

        auto sequence = cocos2d::Sequence::create(duration, fadeOut, nullptr);

        this->runAction(sequence);
    });

}
