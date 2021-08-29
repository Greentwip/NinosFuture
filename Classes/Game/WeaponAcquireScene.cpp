
#include "WeaponAcquireScene.h"
#include "GameStateMachine.h"

#include "Windy/Level.h"

#include "Windy/EntityFactory.h"

#include "Entities/Weapons/VioletBullet.h"
#include "Entities/Weapons/ExtremeBullet.h"
#include "Entities/Weapons/SheriffBullet.h"

#include "Windy/Entities/Bounds.h"

#include "Entities/UI/EnergyBar.h"
#include "Entities/UI/GameGui.h"

#include "Entities/Player/GamePlayer.h"

#include "Entities/Logic/GameLevelController.h"

#include "GameManager.h"

#include "Intro.h"

#include "Game/Entities/UI/Fader.h"

using namespace game;
using namespace cocos2d;

class WeaponAcquireSceneResources {
public:
    static std::string resourcesRootPath;
    static std::string tilemapRootPath;
};

#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
std::string WeaponAcquireSceneResources::resourcesRootPath = "Assets/Resources/res";
#else
std::string WeaponAcquireSceneResources::resourcesRootPath = "res";
#endif

std::string WeaponAcquireSceneResources::tilemapRootPath = "tilemaps";



Scene* WeaponAcquireScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    WeaponAcquireScene* layer = WeaponAcquireScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool WeaponAcquireScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }

    _targetBrownerId = -1;
    _morphing = false;
    _playerActionsFinished = false;

    _exitTimer = 0;
    _exitTimeDelay = 2;

    _exitingScreen = false;
    _transitioning = false;

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    windy::EntityFactory::getInstance().clear();

    GamePlayer::getResources().cache();
    VioletBullet::getResources(windy::Browner::ChargePower::low).cache();
    VioletBullet::getResources(windy::Browner::ChargePower::mid).cache();
    VioletBullet::getResources(windy::Browner::ChargePower::high).cache();
    ExtremeBullet::getResources().cache();
    SheriffBullet::getResources().cache();
    
    EnergyBar::preloadResources();


    windy::EntityFactory::getInstance().registerType<GamePlayer>("player");
    windy::EntityFactory::getInstance().registerType<EnergyBar>("energy_bar");
    windy::EntityFactory::getInstance().registerType<GameGui>("gui");

    windy::EntityFactory::getInstance().registerType<GameLevelController>("level_controller");

    auto level = 
        windy::Level::create(
            WeaponAcquireSceneResources::resourcesRootPath,
            WeaponAcquireSceneResources::tilemapRootPath,
            "weapon",
            windy::Sounds::NONE);

    this->addChild(level);

    _player = dynamic_cast<GamePlayer*>(level->player);

    _player->setAcquiringWeapon(true);


    auto levelMug = GameManager::getInstance().currentLevel->mug;

    int brownerId = -1;

    if (levelMug.compare("freezerman") == 0) {
        brownerId = GameManager::getInstance().browners.freezer->id;
    }
    else if (levelMug.compare("sheriffman") == 0) {
        brownerId = GameManager::getInstance().browners.sheriff->id;
    }
    else if (levelMug.compare("boomerman") == 0) {
        brownerId = GameManager::getInstance().browners.boomer->id;
    }
    else if (levelMug.compare("militaryman") == 0) {
        brownerId = GameManager::getInstance().browners.military->id;
    }
    else if (levelMug.compare("vineman") == 0) {
        brownerId = GameManager::getInstance().browners.vine->id;
    }
    else if (levelMug.compare("shieldman") == 0) {
        brownerId = GameManager::getInstance().browners.shield->id;
    }
    else if (levelMug.compare("nightman") == 0) {
        brownerId = GameManager::getInstance().browners.night->id;
    }
    else if (levelMug.compare("torchman") == 0) {
        brownerId = GameManager::getInstance().browners.torch->id;
    }

    if (brownerId == -1) {
        // currentLevel was not set or has an unknown mug
        throw std::exception();
    }

    _targetBrownerId = brownerId;

    
    level->setPaused(true, true);
    
    auto intro = Intro::create("cody", [=]() { level->setPaused(false, true); });

    level->addChild(intro, 128);

    intro->setPositionY(intro->getPositionY() + 32);

    auto fader = Fader::create(cocos2d::Point(0, 0));

    fader->setPosition(cocos2d::Point(
        fader->getSprite()->getContentSize().width * 0.5f * -1, 
        fader->getSprite()->getContentSize().height * 0.5f * -1));

    fader->setOpacity(255);

    level->bounds->addChild(fader, 4096);

    _fader = fader;

    _fader->fadeOut(nullptr);



    return true;
}


void WeaponAcquireScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void WeaponAcquireScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void WeaponAcquireScene::update(float dt)
{
    _player->canMove = false;

    if (_player->onGround) {
        if (!_morphing) {
            _morphing = true;
            _player->morph(true);

            auto morphDelay = cocos2d::DelayTime::create(_player->currentBrowner->getActionDuration("morph"));
            auto morphCallback = cocos2d::CallFunc::create([this]() {
                _player->morph(false);
                _player->switchBrowner(_targetBrownerId);
            });
            auto postMorphDelay = cocos2d::DelayTime::create(1);
            auto attackCallback = cocos2d::CallFunc::create([this]() {
                _player->currentBrowner->timedShoot();
            });
            auto attackDelay = cocos2d::DelayTime::create(4);
            auto finalCallback = cocos2d::CallFunc::create([this]() {
                _playerActionsFinished = true;
                _player->exit();
            });

            auto sequence = 
                cocos2d::Sequence::create(morphDelay, morphCallback, postMorphDelay, attackCallback, attackDelay, finalCallback, nullptr);

            runAction(sequence);

        }
    }

    if (_playerActionsFinished && !_exitingScreen) {
        switch (_player->screenState) 
        {
            case windy::Player::ScreenState::OffScreen: 
            {
                _exitTimer = _exitTimeDelay;
                _exitingScreen = true;
            }
            break;
        }
    }
    else if (_exitingScreen && !_transitioning) {
        if (_exitTimer <= 0) {
            _exitTimer = 0;
            _transitioning = true;

            _fader->fadeIn([this]() {
                GameStateMachine::getInstance().pushState(GameState::StageSelect);
            });

        }
        else {
            _exitTimer -= dt;
        }
    }
}
