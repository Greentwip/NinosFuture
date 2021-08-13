#include "GamePlayer.h"

#include "Windy/Entities/Browner.h"
#include "Windy/Armature.h"
#include "Windy/Sprite.h"
#include "Windy/GameTags.h"
#include "Windy/SaveManager.h"
#include "Windy/Input.h"

#include "Game/Entities/UI/GameGui.h"
#include "Game/Entities/UI/EnergyBar.h"
#include "Game/GameManager.h"

#include "TeleportBrowner.h"
#include "HelmetBrowner.h"
#include "VioletBrowner.h"
#include "SheriffBrowner.h"



using namespace game;


class PlayerResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string PlayerResources::spritePath = "sprites/characters/player/regular/browners/browners";
std::string PlayerResources::armaturePath = "physics/characters/player/regular/browners/browners";

void GamePlayer::preloadResources() {
    windy::Armature::cache(PlayerResources::armaturePath);
    windy::Sprite::cache(PlayerResources::spritePath);
}


bool GamePlayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->gui = nullptr;

    auto armature = windy::Armature(PlayerResources::armaturePath);

    auto newAnchor = armature.get("browners").anchor;

    this->sprite = windy::Sprite::create(PlayerResources::spritePath, newAnchor);
    this->addChild(this->sprite);

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();

    this->collisionRectangles = armature.get("browners").collisionRectangles;

    auto collisionBoxCenter = cocos2d::Point(this->collisionRectangles[0]->getMidX(), this->collisionRectangles[0]->getMidY());

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];


    this->sprite->setPosition(collisionBoxCenter + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    this->setTag(windy::GameTags::General::Player);

    this->initVariables();

    this->setupBrowners();

    this->gui = dynamic_cast<GameGui*>(this->level->gui);



    return true;
}

void GamePlayer::onRestart() {

    windy::Input::keys[windy::InputKey::A].status = windy::KeyStatus::Up;

    if (windy::Input::keys[windy::InputKey::A].pressed) {
        windy::Input::keys[windy::InputKey::A].pressed = false;
        windy::Input::keys[windy::InputKey::A].released = false;
    }


    windy::Input::keys[windy::InputKey::B].status = windy::KeyStatus::Up;

    if (windy::Input::keys[windy::InputKey::B].pressed) {
        windy::Input::keys[windy::InputKey::B].pressed = false;
        windy::Input::keys[windy::InputKey::B].released = false;
    }

    this->switchBrowner(GameManager::getInstance().browners.teleport->id);

    this->vulnerable = true;

    this->sprite->setFlippedX(false);
    this->sprite->setVisible(true);
    this->currentBrowner->activate();

    this->alive = true;

    this->restoringHealth = false;
    this->restoringWeaponEnergy = false;

    this->initVariables();
}

void GamePlayer::onSpawn() {

    GameManager::getInstance().options.extremeActivated = false;

    if (GameManager::getInstance().options.helmetActivated) {
        this->switchBrowner(GameManager::getInstance().browners.helmet->id);
    }
    else {
        this->switchBrowner(GameManager::getInstance().browners.violet->id);
    }
}

void GamePlayer::onPlayerExit() {
    this->switchBrowner(GameManager::getInstance().browners.teleport->id);
}

void GamePlayer::switchBrowner(int brownerId) {
    
    if (this->currentBrowner != nullptr) {
        this->currentBrowner->deactivate();
    }

    for (int i = 0; i < this->browners.size(); ++i) {
        auto browner = this->browners.at(i);
        if (browner->brownerId == brownerId) {
            this->currentBrowner = browner;
        }
    }

    this->currentBrowner->activate();

    if (this->currentBrowner->brownerId == GameManager::getInstance().browners.teleport->id ||
        this->currentBrowner->brownerId == GameManager::getInstance().browners.helmet->id ||
        this->currentBrowner->brownerId == GameManager::getInstance().browners.violet->id) {

        this->gui->weaponBar->setVisible(false);
    }
    else {
        this->gui->weaponBar->setVisible(true);
    }

    this->charging = false;
    this->currentBrowner->chargeTimer = 0;
    this->currentBrowner->chargePower = "low";

    this->vulnerable = true;

    this->triggerActions();

}

void GamePlayer::setupBrowners() {
    auto teleportBrowner = windy::Browner::create<TeleportBrowner>(this->level, this);
    auto helmetBrowner = windy::Browner::create<HelmetBrowner>(this->level, this);
    auto violetBrowner = windy::Browner::create<VioletBrowner>(this->level, this);
    auto sheriffBrowner = windy::Browner::create<SheriffBrowner>(this->level, this);


    this->addChild(teleportBrowner);
    this->addChild(helmetBrowner);
    this->addChild(violetBrowner);
    this->addChild(sheriffBrowner);

    this->browners.pushBack(teleportBrowner);
    this->browners.pushBack(helmetBrowner);
    this->browners.pushBack(violetBrowner);
    this->browners.pushBack(sheriffBrowner);

    this->currentBrowner = teleportBrowner;

    this->currentBrowner->runAction("jump");
}

windy::Browner* GamePlayer::getBrowner(int brownerId) {
    windy::Browner* brownerFound = nullptr;
    for (int i = 0; i < this->browners.size(); ++i) {
        auto browner = this->browners.at(i);
        if (browner->brownerId == brownerId) {
            brownerFound = browner;
            break;
        }
    }

    return brownerFound;
}

void GamePlayer::kill(bool killAnimation) {

    if (killAnimation) {

        auto explosionA = cocos2d::CallFunc::create([this]() {
            this->explode(0);
        });

        auto delay = cocos2d::DelayTime::create(0.40f);

        auto explosionB = cocos2d::CallFunc::create([this]() {
            this->explode(22.5f);
        });

        auto killDelay = cocos2d::DelayTime::create(2);

        auto lifeCallback = cocos2d::CallFunc::create([=]() {
            GameManager::getInstance().player.lives -= 1;

            auto slot = GameManager::getInstance().getDefaultSlot();

            slot.lives = GameManager::getInstance().player.lives;

            windy::SaveManager::saveSlot(GameManager::getInstance().slot, slot);

            this->level->restart();
        });

        auto sequence = cocos2d::Sequence::create(explosionA, delay, explosionB, killDelay, lifeCallback, nullptr);

        this->runAction(sequence);
    }
    else {
        this->sprite->setVisible(false);


        auto killDelay = cocos2d::DelayTime::create(2);

        auto lifeCallback = cocos2d::CallFunc::create([=]() {
            GameManager::getInstance().player.lives -= 1;

            auto slot = GameManager::getInstance().getDefaultSlot();

            slot.lives = GameManager::getInstance().player.lives;

            windy::SaveManager::saveSlot(GameManager::getInstance().slot, slot);

            this->level->restart();
        });

        auto sequence = cocos2d::Sequence::create(killDelay, lifeCallback, nullptr);

        this->runAction(sequence);

    }

}

void GamePlayer::restoreHealth(int amount) {
    this->restoringHealth = true;
    this->gui->restoreHealth(amount);

}

void GamePlayer::restoreWeaponEnergy(int amount) {
    
    this->restoringWeaponEnergy = true;

    if (this->currentBrowner->brownerId != GameManager::getInstance().browners.teleport->id &&
        this->currentBrowner->brownerId != GameManager::getInstance().browners.helmet->id &&
        this->currentBrowner->brownerId != GameManager::getInstance().browners.violet->id) {
        this->currentBrowner->restoreWeaponEnergy(amount);
        this->gui->restoreWeapon(amount);
    }


}


void GamePlayer::checkHealth() {
    
    if (!this->restoringHealth) {
        this->gui->healthBar->setValue(this->health);
    }

    if (!this->gui->fillingHealthBar) {
        this->restoringHealth = false;
    }

    if (!this->restoringWeaponEnergy) {
        this->gui->weaponBar->setValue(this->currentBrowner->energy);
    }

    if (!this->gui->fillingWeaponBar) {
        this->restoringWeaponEnergy = false;
    }

    

    Player::checkHealth();
}

void GamePlayer::onUpdate(float dt) {
    Player::onUpdate(dt);
}

