
#include "GameScene.h"
#include "GameStateMachine.h"
#include "GameManager.h"

#include "Windy/Level.h"

#include "Windy/Sprite.h"

#include "Windy/Armature.h"

#include "Windy/EntityFactory.h"

#include "Windy/AudioManager.h"

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
#include "Game/Entities/Enemies/Military/GreenSoldier.h"
#include "Game/Entities/Enemies/Military/RedSoldier.h"
#include "Game/Entities/Enemies/Military/KnifeSoldier.h"

#include "Game/Entities/Enemies/Night/BackPacker.h"
#include "Game/Entities/Enemies/Night/Bat.h"
#include "Game/Entities/Enemies/Night/FallingSkull.h"
#include "Game/Entities/Enemies/Night/FlashBrain.h"
#include "Game/Entities/Enemies/Night/Golem.h"
#include "Game/Entities/Enemies/Night/Rip.h"
#include "Game/Entities/Enemies/Night/Spider.h"
#include "Game/Entities/Enemies/Night/WallSkull.h"
#include "Game/Entities/Enemies/Night/Worm.h"

#include "Game/Entities/Boss/SheriffMan.h"

#include "Game/Entities/Items/GameItem.h"

#include "Game/Entities/Player/GamePlayer.h"

#include "Game/Entities/Weapons/VioletBullet.h"
#include "Game/Entities/Weapons/ExtremeBullet.h"
#include "Game/Entities/Weapons/SheriffBullet.h"
#include "Game/Entities/Weapons/DirectionalBullet.h"
#include "Game/Entities/Weapons/TremorLaser.h"
#include "Game/Entities/Weapons/TankBullet.h"
#include "Game/Entities/Weapons/TremorDrill.h"
#include "Game/Entities/Weapons/Grenade.h"
#include "Game/Entities/Weapons/Knife.h"
#include "Game/Entities/Weapons/BackPackBullet.h"


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

    auto* currentLevel = GameManager::getInstance().currentLevel.get();
    if (currentLevel && currentLevel->mug.compare("sheriffman") == 0) {

        Tremor::getResources().cache();
        Cow::getResources().cache();
        Barrel::getResources().cache();
        JetBird::getResources().cache();
        TremorDrill::getResources().cache();
        TremorLaser::getResources().cache();
        CannonJoe::getResources().cache();
        Taban::getResources().cache();
        Sumatran::getResources().cache();
        DirectionalBullet::getResources().cache();

        windy::AudioManager::cacheSound(windy::Sounds::SheriffMan);

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

        Subeil::getResources().cache();
        Lyric::getResources().cache();
        Taban::getResources().cache();
        Sumatran::getResources().cache();
        CannonJoe::getResources().cache();
        
        DirectionalBullet::getResources().cache();

        windy::AudioManager::cacheSound(windy::Sounds::VineMan);
        
        windy::EntityFactory::getInstance().registerType<Subeil>("subeil");
        windy::EntityFactory::getInstance().registerType<Lyric>("lyric");
        windy::EntityFactory::getInstance().registerType<Taban>("taban");
        windy::EntityFactory::getInstance().registerType<Sumatran>("sumatran");
        windy::EntityFactory::getInstance().registerType<CannonJoe>("cannon_joe");
        
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Subeil>("subeil");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Lyric>("lyric");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Taban>("taban");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Sumatran>("sumatran");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<CannonJoe>("cannon_joe");
        

    }
    else if (currentLevel && currentLevel->mug.compare("militaryman") == 0) {

        GamePlatform::getResources().cache();
        RollRunner::getResources().cache();
        Tank::getResources().cache();
        GreenSoldier::getResources().cache();
        RedSoldier::getResources().cache();
        KnifeSoldier::getResources().cache();
        TankBullet::getResources().cache();
        Grenade::getResources().cache();
        Knife::getResources().cache();

        windy::AudioManager::cacheSound(windy::Sounds::MilitaryMan);

        windy::EntityFactory::getInstance().registerType<GamePlatform>("platform");
        windy::EntityFactory::getInstance().registerType<RollRunner>("roll_runner");
        windy::EntityFactory::getInstance().registerType<Tank>("tank");
        windy::EntityFactory::getInstance().registerType<GreenSoldier>("green_soldier");
        windy::EntityFactory::getInstance().registerType<RedSoldier>("red_soldier");
        windy::EntityFactory::getInstance().registerType<KnifeSoldier>("claw_soldier");

        windy::EntityFactory::getInstance().registerTypeCollisionFunc<GamePlatform>("platform");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<RollRunner>("roll_runner");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Tank>("tank");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<GreenSoldier>("green_soldier");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<RedSoldier>("red_soldier");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<KnifeSoldier>("claw_soldier");
    }
    else if (currentLevel && currentLevel->mug.compare("nightman") == 0) {
        BackPacker::getResources().cache();
        Bat::getResources().cache();
        FallingSkull::getResources().cache();
        FlashBrain::getResources().cache();
        Golem::getResources().cache();
        Rip::getResources().cache();
        Spider::getResources().cache();
        WallSkull::getResources().cache();
        Worm::getResources().cache();

        BackPackBullet::getResources().cache();

        GameFallingBlock::getResources().cache();

        windy::AudioManager::cacheSound(windy::Sounds::NightMan);

        windy::EntityFactory::getInstance().registerType<BackPacker>("backpacker");
        windy::EntityFactory::getInstance().registerType<Bat>("batcombat");
        windy::EntityFactory::getInstance().registerType<FallingSkull>("falling_skull");
        windy::EntityFactory::getInstance().registerType<FlashBrain>("flash_brain");
        windy::EntityFactory::getInstance().registerType<Golem>("golem_jumper");
        windy::EntityFactory::getInstance().registerType<Rip>("rip_jumper");
        windy::EntityFactory::getInstance().registerType<Spider>("wall_spider");
        windy::EntityFactory::getInstance().registerType<WallSkull>("wall_skull");
        windy::EntityFactory::getInstance().registerType<Worm>("robot_worm");

        windy::EntityFactory::getInstance().registerType<GameFallingBlock>("falling_block");

        windy::EntityFactory::getInstance().registerTypeCollisionFunc<BackPacker>("backpacker");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Bat>("batcombat");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<FallingSkull>("falling_skull");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<FlashBrain>("flash_brain");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Golem>("golem_jumper");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Rip>("rip_jumper");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Spider>("wall_spider");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<WallSkull>("wall_skull");
        windy::EntityFactory::getInstance().registerTypeCollisionFunc<Worm>("robot_worm");

        windy::EntityFactory::getInstance().registerTypeCollisionFunc<GameFallingBlock>("falling_block");

    }

    if (currentLevel) {

        auto levelMug = GameManager::getInstance().currentLevel->mug;

        windy::Sounds levelBGM = windy::Sounds::NONE;

        if (levelMug.compare("freezerman") == 0) {
            //levelBGM = windy::Sounds::FreezerMan;
        }
        else if (levelMug.compare("sheriffman") == 0) {
            levelBGM = windy::Sounds::SheriffMan;
        }
        else if (levelMug.compare("boomerman") == 0) {
            //levelBGM = windy::Sounds::BoomerMan;
        }
        else if (levelMug.compare("militaryman") == 0) {
            levelBGM = windy::Sounds::MilitaryMan;
        }
        else if (levelMug.compare("vineman") == 0) {
            levelBGM = windy::Sounds::VineMan;
        }
        else if (levelMug.compare("shieldman") == 0) {
            //levelBGM = windy::Sounds::ShieldMan;
        }
        else if (levelMug.compare("nightman") == 0) {
            levelBGM = windy::Sounds::NightMan;
        }
        else if (levelMug.compare("torchman") == 0) {
            //levelBGM = windy::Sounds::TorchMan;
        }

        auto level = windy::Level::create(GameSceneResources::resourcesRootPath,
                                          GameSceneResources::tilemapRootPath,
                                          currentLevel->mug,
                                          levelBGM);

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
