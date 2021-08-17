#include "GamePlayer.h"

#include "ExtremeBrowner.h"
#include "HelmetBrowner.h"
#include "SheriffBrowner.h"
#include "TeleportBrowner.h"
#include "VioletBrowner.h"
#include "Game/GameManager.h"
#include "Game/Entities/Items/ItemFlags.h"
#include "Game/Entities/UI/EnergyBar.h"
#include "Game/Entities/UI/GameGui.h"
#include "Windy/AudioManager.h"
#include "Windy/Armature.h"
#include "Windy/GameTags.h"
#include "Windy/Input.h"
#include "Windy/SaveManager.h"
#include "Windy/Sprite.h"
#include "Windy/Entities/Browner.h"
#include "Windy/Entities/Item.h"

using namespace game;

windy::Resources& GamePlayer::getResources() {
    static windy::Resources resources{windy::ResourceKind::Player, "browners"};
    return resources;
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

    const auto& resources = getResources();

    auto armature = windy::Armature(resources._armaturePath);

    auto newAnchor = armature.get(resources._entityName).anchor;

    this->sprite = windy::Sprite::create(resources._spritePath, newAnchor);
    this->addChild(this->sprite);

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();

    this->collisionRectangles = armature.get(resources._entityName).collisionRectangles;

    auto collisionBoxCenter = cocos2d::Point(this->collisionRectangles[0]->getMidX(), this->collisionRectangles[0]->getMidY());

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        auto nodePosition = this->getPosition();
        auto rectangleOrigin = this->collisionRectangles[i]->origin;

        auto normalizedRectangle = std::make_shared<cocos2d::Rect>(*this->collisionRectangles[i]);
        normalizedRectangle->origin.x = nodePosition.x + rectangleOrigin.x;
        normalizedRectangle->origin.y = nodePosition.y + rectangleOrigin.y;

        this->collisionRectangles[i] = normalizedRectangle;
    }

    this->collisionBox = this->collisionRectangles[0];


    this->sprite->setPosition(collisionBoxCenter + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    this->setTag(windy::GameTags::General::Player);

    this->initVariables();
    this->setupBrowners();

    this->weaponTag = windy::GameTags::WeaponPlayer;

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
        if (this->currentBrowner->brownerId == brownerId) {
            return;
        }
    }

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
    this->currentBrowner->chargePower = ExtremeBrowner::ChargePower::low;

    this->vulnerable = true;

    this->triggerActions();

}

void GamePlayer::setupBrowners() {
    auto teleportBrowner = windy::Browner::create<TeleportBrowner>(this->level, this);
    auto helmetBrowner = windy::Browner::create<HelmetBrowner>(this->level, this);
    auto violetBrowner = windy::Browner::create<VioletBrowner>(this->level, this);
    auto extremeBrowner = windy::Browner::create<ExtremeBrowner>(this->level, this);
    auto sheriffBrowner = windy::Browner::create<SheriffBrowner>(this->level, this);


    this->addChild(teleportBrowner);
    this->addChild(helmetBrowner);
    this->addChild(violetBrowner);
    this->addChild(extremeBrowner);
    this->addChild(sheriffBrowner);

    this->browners.pushBack(teleportBrowner);
    this->browners.pushBack(helmetBrowner);
    this->browners.pushBack(violetBrowner);
    this->browners.pushBack(extremeBrowner);
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

            windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

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

            windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

            this->level->restart();
        });

        auto sequence = cocos2d::Sequence::create(killDelay, lifeCallback, nullptr);

        this->runAction(sequence);

    }

}


void GamePlayer::onItemAcquired(windy::Item* item) {


    if (item->collectible) {

        bool found = false;

        auto iterator = std::find_if(
            GameManager::getInstance().collectibles.begin(), 
            GameManager::getInstance().collectibles.end(), 
            [=](const std::pair<std::string, bool> collectible) {
                return collectible.first == item->name;
            });

        if (iterator == GameManager::getInstance().collectibles.end()) {
            GameManager::getInstance().collectibles.push_back({ item->name, true });
        }
        
        auto slot = GameManager::getInstance().getDefaultSlot();

        slot.collectibles = GameManager::getInstance().collectibles;

        windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);
    }

    switch (item->id) 
    {
        case ItemId::Life:
        {
            GameManager::getInstance().player.lives += 1;

            if (GameManager::getInstance().player.lives >= 9) {
                GameManager::getInstance().player.lives = 9;
            }

            auto slot = GameManager::getInstance().getDefaultSlot();

            slot.lives = GameManager::getInstance().player.lives;

            windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

            windy::AudioManager::playSfx(windy::Sounds::GetLife);


        }
        break;

        case ItemId::Helmet:
        case ItemId::Head:
        case ItemId::Chest:
        case ItemId::Fist:
        case ItemId::Boot: 
        {
            std::shared_ptr<UnlockableData> unlockable = nullptr;
            auto unlockables = GameManager::getInstance().unlockables.collection;

            for (int i = 0; i < unlockables.size(); ++i) {
                auto unlockableItem = unlockables[i];
                if (item->id == unlockableItem->id) {
                    unlockable = unlockableItem;
                    unlockable->acquired = true;
                    break;
                }
            }

            auto slot = GameManager::getInstance().getDefaultSlot();

            switch (item->id) {
                case ItemId::Helmet:
                    slot.helmet = true;
                    break;
                case ItemId::Head:
                    slot.head = true;
                    break;
                case ItemId::Chest:
                    slot.chest = true;
                    break;
                case ItemId::Fist:
                    slot.fist = true;
                    break;
                case ItemId::Boot:
                    slot.boot = true;
                    break;

            };

            if (item->id == ItemId::Head) {
                slot.helmet = true;
                GameManager::getInstance().unlockables.helmet->acquired = true;
                GameManager::getInstance().browners.helmet->acquired = true;
            }

            
            bool extremeAcquired = true;

            for (int i = 0; i < unlockables.size(); ++i) {
                auto unlockableItem = unlockables[i];
                if (!unlockableItem->acquired) {
                    extremeAcquired = false;
                    break;
                }
            }

            if (extremeAcquired) {
                GameManager::getInstance().browners.extreme->acquired = true;
            }


            windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

            windy::AudioManager::playSfx(windy::Sounds::GetLife);

        }
            break;

        case ItemId::HealthSmall: 
        {
            this->restoreHealth(HealthPower::HealthSmallPower);
        }
            break;

        case ItemId::HealthBig: 
        {
            this->restoreHealth(HealthPower::HealthBigPower);
        }
            break;

        case ItemId::EnergySmall: 
        {
            this->restoreWeaponEnergy(EnergyPower::EnergySmallPower);
        }
            break;

        case ItemId::EnergyBig:
        {
            this->restoreWeaponEnergy(EnergyPower::EnergyBigPower);
        }
            break;

        case ItemId::eTank:
        {
            GameManager::getInstance().player.eTanks += 1;

            if (GameManager::getInstance().player.eTanks >= 9) {
                GameManager::getInstance().player.eTanks = 9;
            }

            auto slot = GameManager::getInstance().getDefaultSlot();

            slot.e = GameManager::getInstance().player.eTanks;

            windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

            windy::AudioManager::playSfx(windy::Sounds::GetLife);


        }
            break;

        case ItemId::mTank:
        {
            GameManager::getInstance().player.mTanks += 1;

            if (GameManager::getInstance().player.mTanks >= 9) {
                GameManager::getInstance().player.mTanks = 9;
            }

            auto slot = GameManager::getInstance().getDefaultSlot();

            slot.m = GameManager::getInstance().player.mTanks;

            windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

            windy::AudioManager::playSfx(windy::Sounds::GetLife);


        }
            break;
    }

    item->finishForever();
    //item->onFinished();
}


void GamePlayer::restoreHealth(int amount) {

    if (this->health < this->maxHealth) {
        this->health += amount;

        if (this->health >= this->maxHealth) {
            this->health = this->maxHealth;
        }

        this->restoringHealth = true;
        this->gui->restoreHealth(amount);
    }
    
}

void GamePlayer::restoreWeaponEnergy(int amount) {
    
    this->restoringWeaponEnergy = true;

    if (this->currentBrowner->brownerId != GameManager::getInstance().browners.teleport->id &&
        this->currentBrowner->brownerId != GameManager::getInstance().browners.helmet->id &&
        this->currentBrowner->brownerId != GameManager::getInstance().browners.violet->id) {
        if (this->currentBrowner->energy < this->maxWeaponEnergy) {
            this->currentBrowner->restoreWeaponEnergy(amount);
            this->gui->restoreWeapon(amount);
        }
        
    }


}


void GamePlayer::checkHealth() {
    
    if (!this->restoringHealth) {
        this->gui->healthBar->setValue(this->health - 1);
    }

    if (!this->gui->fillingHealthBar) {
        this->restoringHealth = false;
    }

    if (!this->restoringWeaponEnergy) {
        this->gui->weaponBar->setValue(this->currentBrowner->energy - 1);
    }

    if (!this->gui->fillingWeaponBar) {
        this->restoringWeaponEnergy = false;
    }

    

    Player::checkHealth();
}

void GamePlayer::onUpdate(float dt) {
    Player::onUpdate(dt);
}
