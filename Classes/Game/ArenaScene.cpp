
#include "ArenaScene.h"
#include "GameStateMachine.h"
#include "GameManager.h"

#include "Windy/Level.h"

#include "Windy/Sprite.h"

#include "Windy/Armature.h"

#include "Windy/EntityFactory.h"

#include "Windy/AudioManager.h"

#include "Game/Entities/Boss/GameBoss.h"
#include "Game/Entities/Boss/SheriffMan.h"
#include "Game/Entities/Boss/MilitaryMan.h"
#include "Game/Entities/Boss/VineMan.h"
#include "Game/Entities/Boss/NightMan.h"

#include "Game/Entities/Items/GameItem.h"

#include "Game/Entities/Player/GamePlayer.h"

#include "Game/Entities/Weapons/VioletBullet.h"
#include "Game/Entities/Weapons/ExtremeBullet.h"
#include "Game/Entities/Weapons/SheriffBullet.h"
#include "Game/Entities/Weapons/MilitaryMissile.h"
#include "Game/Entities/Weapons/VineBall.h"
#include "Game/Entities/Weapons/Shiki108.h"

#include "Game/Entities/Special/GameVerticalDoor.h"
#include "Game/Entities/Special/GameHorizontalDoor.h"
#include "Game/Entities/Special/GamePlatform.h"
#include "Game/Entities/Special/GameFallingBlock.h"

#include "Game/Entities/UI/EnergyBar.h"
#include "Game/Entities/UI/GameGui.h"
#include "Game/Entities/UI/ReadyIndicator.h"
#include "Game/Entities/UI/Pause/PauseAnimation.h"
#include "Game/Entities/UI/Pause/PauseInterruptor.h"
#include "Game/Entities/UI/Pause/PauseMenu.h"
#include "Game/Entities/UI/Pause/PauseSelector.h"

#include "Game/Entities/Scenery/General/GameExplosion.h"

#include "Game/Entities/Logic/GameLevelController.h"



using namespace game;
using namespace cocos2d;

class ArenaSceneResources {
public:
    static std::string resourcesRootPath;
    static std::string tilemapRootPath;
};

#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
std::string ArenaSceneResources::resourcesRootPath = "Assets/Resources/res";
#else
std::string ArenaSceneResources::resourcesRootPath = "res";
#endif

std::string ArenaSceneResources::tilemapRootPath = "tilemaps";



Scene* ArenaScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    ArenaScene* layer = ArenaScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool ArenaScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();
    windy::AudioManager::uncacheAll();

    windy::AudioManager::cacheSound(windy::Sounds::Select);
    windy::AudioManager::cacheSound(windy::Sounds::Selected);
    windy::AudioManager::cacheSound(windy::Sounds::Door);
    windy::AudioManager::cacheSound(windy::Sounds::BusterLow);
    windy::AudioManager::cacheSound(windy::Sounds::BusterMid);
    windy::AudioManager::cacheSound(windy::Sounds::BusterHigh);
    windy::AudioManager::cacheSound(windy::Sounds::BusterChargingMid);
    windy::AudioManager::cacheSound(windy::Sounds::BusterChargingHigh);
    windy::AudioManager::cacheSound(windy::Sounds::Land);
    windy::AudioManager::cacheSound(windy::Sounds::Explosion1);
    windy::AudioManager::cacheSound(windy::Sounds::EnemyHit);
    windy::AudioManager::cacheSound(windy::Sounds::Roar);
    windy::AudioManager::cacheSound(windy::Sounds::Taban);
    windy::AudioManager::cacheSound(windy::Sounds::PlayerHit);
    windy::AudioManager::cacheSound(windy::Sounds::Death);
    windy::AudioManager::cacheSound(windy::Sounds::Teleport1);
    windy::AudioManager::cacheSound(windy::Sounds::Teleport2);
    windy::AudioManager::cacheSound(windy::Sounds::BossTheme);
    windy::AudioManager::cacheSound(windy::Sounds::Victory);
    windy::AudioManager::cacheSound(windy::Sounds::GetEnergy);
    windy::AudioManager::cacheSound(windy::Sounds::Error);
    windy::AudioManager::cacheSound(windy::Sounds::GetLife);
    windy::AudioManager::cacheSound(windy::Sounds::LevelComplete);
    windy::AudioManager::cacheSound(windy::Sounds::Pause);

    windy::EntityFactory::getInstance().clear();

    GameBoss::getResources().cache();
    GamePlayer::getResources().cache();
    GameItem::getResources().cache();
    GameVerticalDoor::getResources().cache();
    GameHorizontalDoor::getResources().cache();
    VioletBullet::getResources(windy::Browner::ChargePower::low).cache();
    VioletBullet::getResources(windy::Browner::ChargePower::mid).cache();
    VioletBullet::getResources(windy::Browner::ChargePower::high).cache();
    ExtremeBullet::getResources().cache();
    SheriffBullet::getResources().cache();
    MilitaryMissile::getResources().cache();
    VineBall::getResources().cache();
    Shiki108::getResources().cache();

    EnergyBar::preloadResources();

    GameExplosion::getResources().cache();
    PauseInterruptor::getResources().cache();
    PauseAnimation::getResources().cache();

    PauseMenu::preloadResources();
    PauseSelector::preloadResources();
    ReadyIndicator::preloadResources();


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


    windy::EntityFactory::getInstance().registerType<SheriffMan>("sheriff");
    windy::EntityFactory::getInstance().registerTypeCollisionFunc<SheriffMan>("sheriff");

    windy::EntityFactory::getInstance().registerType<MilitaryMan>("military");
    windy::EntityFactory::getInstance().registerTypeCollisionFunc<MilitaryMan>("military");

    windy::EntityFactory::getInstance().registerType<VineMan>("vine");
    windy::EntityFactory::getInstance().registerTypeCollisionFunc<VineMan>("vine");

    windy::EntityFactory::getInstance().registerType<NightMan>("night");
    windy::EntityFactory::getInstance().registerTypeCollisionFunc<NightMan>("night");


    auto level = windy::Level::create(ArenaSceneResources::resourcesRootPath,
        ArenaSceneResources::tilemapRootPath,
        "arena",
        windy::Sounds::NONE);

    this->addChild(level);

    return true;
}


void ArenaScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void ArenaScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void ArenaScene::update(float dt)
{
}
