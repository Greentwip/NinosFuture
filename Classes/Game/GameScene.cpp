
#include "GameScene.h"
#include "GameStateMachine.h"
#include "GameManager.h"

#include "Windy/Level.h"

#include "Windy/Sprite.h"

#include "Windy/Armature.h"

#include "Windy/EntityFactory.h"


#include "Game/Entities/Enemies/Common/CannonJoe.h"
#include "Game/Entities/Enemies/Common/Subeil.h"
#include "Game/Entities/Enemies/Common/Taban.h"
#include "Game/Entities/Enemies/Common/Lyric.h"
#include "Game/Entities/Enemies/Common/Sumatran.h"

#include "Game/Entities/Enemies/Sheriff/Tremor.h"
#include "Game/Entities/Enemies/Sheriff/Cow.h"
#include "Game/Entities/Enemies/Sheriff/Barrel.h"
#include "Game/Entities/Enemies/Sheriff/JetBird.h"

#include "Game/Entities/Enemies/Military/RollRunner.h"
#include "Game/Entities/Enemies/Military/Tank.h"

#include "Game/Entities/Boss/SheriffMan.h"

#include "Game/Entities/Items/GameItem.h"

#include "Game/Entities/Player/GamePlayer.h"

#include "Game/Entities/Special/GameVerticalDoor.h"
#include "Game/Entities/Special/GameHorizontalDoor.h"

#include "Game/Entities/UI/EnergyBar.h"
#include "Game/Entities/UI/GameGui.h"
#include "Game/Entities/UI/Pause/PauseAnimation.h"
#include "Game/Entities/UI/Pause/PauseInterruptor.h"
#include "Game/Entities/UI/Pause/PauseMenu.h"
#include "Game/Entities/UI/Pause/PauseSelector.h"

#include "Game/Entities/Scenery/General/GameExplosion.h"

#include "Game/Entities/Logic/GameLevelController.h"



using namespace game;
using namespace cocos2d;

class GameSceneResources {
public:
    static std::string resourcesRootPath;
    static std::string tilemapRootPath;
};

#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
std::string GameSceneResources::resourcesRootPath = "Assets/Resources/res";
#else
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

    windy::EntityFactory::getInstance().clear();

    windy::EntityFactory::getInstance().registerType<GamePlayer>("player");
    windy::EntityFactory::getInstance().registerType<GameItem>("item");
    windy::EntityFactory::getInstance().registerType<GameVerticalDoor>("door");
    windy::EntityFactory::getInstance().registerType<GameHorizontalDoor>("horizontal_door");
    windy::EntityFactory::getInstance().registerType<EnergyBar>("energy_bar");
    windy::EntityFactory::getInstance().registerType<GameGui>("gui");
    windy::EntityFactory::getInstance().registerType<GameExplosion>("explosion");
    windy::EntityFactory::getInstance().registerType<GameLevelController>("level_controller");

    windy::EntityFactory::getInstance().registerTypeCollisionFunc<GameExplosion>("explosion");
    windy::EntityFactory::getInstance().registerTypeCollisionFunc<GameItem>("item");

    auto* currentLevel = GameManager::getInstance().currentLevel.get();
    if (currentLevel && currentLevel->mug.compare("sheriffman") == 0) {

        windy::EntityFactory::getInstance().registerType<SheriffMan>("sheriff");
        windy::EntityFactory::getInstance().registerType<Tremor>("tremor");
        windy::EntityFactory::getInstance().registerType<Cow>("cow");
        windy::EntityFactory::getInstance().registerType<Barrel>("barrel");
        windy::EntityFactory::getInstance().registerType<JetBird>("jetbird");
        windy::EntityFactory::getInstance().registerType<CannonJoe>("cannon_joe");
        windy::EntityFactory::getInstance().registerType<Taban>("taban");
        windy::EntityFactory::getInstance().registerType<Sumatran>("sumatran");

        windy::EntityFactory::getInstance().registerTypeCollisionFunc<SheriffMan>("sheriff");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Tremor>("tremor");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Cow>("cow");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Barrel>("barrel");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<JetBird>("jetbird");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<CannonJoe>("cannon_joe");        
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Taban>("taban");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Sumatran>("sumatran");

    }    
    else if (currentLevel && currentLevel->mug.compare("vineman") == 0) {

        windy::EntityFactory::getInstance().registerType<Subeil>("subeil");
        windy::EntityFactory::getInstance().registerType<Lyric>("lyric");
        windy::EntityFactory::getInstance().registerType<Taban>("taban");
        windy::EntityFactory::getInstance().registerType<Sumatran>("sumatran");

        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Subeil>("subeil");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Lyric>("lyric");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Taban>("taban");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Sumatran>("sumatran");

    }
    else if (currentLevel && currentLevel->mug.compare("militaryman") == 0) {
        windy::EntityFactory::getInstance().registerType<RollRunner>("roll_runner");
        windy::EntityFactory::getInstance().registerType<Tank>("tank");

        windy::EntityFactory::getInstance().registerTypeCollisionFunc<RollRunner>("roll_runner");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Tank>("tank");
    }

    if (currentLevel) {
        auto level = windy::Level::create(GameSceneResources::resourcesRootPath,
                                          GameSceneResources::tilemapRootPath,
                                          currentLevel->mug,
                                          windy::Sounds::NONE);

        this->addChild(level);
    }

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
