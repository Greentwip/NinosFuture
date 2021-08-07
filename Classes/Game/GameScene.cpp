
#include "GameScene.h"
#include "GameStateMachine.h"
#include "GameManager.h"

#include "Windy/Level.h"

#include "Windy/Sprite.h"

#include "Windy/Armature.h"

#include "Windy/EntityFactory.h"


#include "Game/Entities/Enemies/Common/CannonJoe.h"
#include "Game/Entities/Enemies/Sheriff/Tremor.h"
#include "Game/Entities/Items/GameItem.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Game/Entities/Special/GameVerticalDoor.h"
#include "Game/Entities/Special/GameHorizontalDoor.h"

#include "Game/Entities/Weapons/DirectionalBullet.h"
#include "Game/Entities/Weapons/TremorDrill.h"
#include "Game/Entities/Weapons/TremorLaser.h"
#include "Game/Entities/Weapons/VioletBullet.h"


using namespace game;
using namespace cocos2d;

class GameSceneResources {
public:
    static std::string resourcesRootPath;
    static std::string tilemapRootPath;
};

#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
std::string GameSceneResources::resourcesRootPath = "Assets/Resources/res";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
std::string GameSceneResources::resourcesRootPath = "res";
#endif

std::string GameSceneResources::tilemapRootPath = "tilemaps";



Scene* GameScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    GameScene* layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    auto fadeIn = FadeIn::create(1.0f);

    this->runAction(fadeIn);

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    windy::Logical::preloadResources<GamePlayer>();
    windy::Logical::preloadResources<Tremor>();
    windy::Logical::preloadResources<CannonJoe>();
    windy::Logical::preloadResources<GameItem>();
    windy::Logical::preloadResources<GameVerticalDoor>();
    windy::Logical::preloadResources<GameHorizontalDoor>();

    windy::Logical::preloadResources<DirectionalBullet>();
    windy::Logical::preloadResources<TremorDrill>();
    windy::Logical::preloadResources<TremorLaser>();
    windy::Logical::preloadResources<VioletBullet>();

    windy::EntityFactory::getInstance().clear();

    windy::EntityFactory::getInstance().registerType<GamePlayer>("player");
    windy::EntityFactory::getInstance().registerType<Tremor>("tremor");
    windy::EntityFactory::getInstance().registerType<CannonJoe>("cannon_joe");
    windy::EntityFactory::getInstance().registerType<GameItem>("item");
    windy::EntityFactory::getInstance().registerType<GameVerticalDoor>("door");
    windy::EntityFactory::getInstance().registerType<GameHorizontalDoor>("horizontal_door");

    windy::EntityFactory::getInstance().registerTypeCollisionFunc<CannonJoe>("cannon_joe");
    windy::EntityFactory::getInstance().registerTypeCollisionFunc<Tremor>("tremor");

    
    auto level = windy::Level::create(GameSceneResources::resourcesRootPath,
                                      GameSceneResources::tilemapRootPath,
                                      GameManager::getInstance().currentLevel->mug, 
                                      windy::Sounds::NONE);

    this->addChild(level);


    return true;
}


void GameScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void GameScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void GameScene::update(float dt)
{
}
