#include <string>

#include "GameGui.h"
#include "EnergyBar.h"

#include "Windy/Level.h"
#include "Windy/Entities/Bounds.h"

#include "Windy/EntityFactory.h"
#include "Windy/AudioManager.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;

void GameGui::build()
{
    auto barBasePosition = cocos2d::Point(this->level->bounds->collisionBox->size.width * -0.5f, this->level->bounds->collisionBox->size.height * 0.5f);
    auto bossBarBasePosition = cocos2d::Point(this->level->bounds->collisionBox->size.width * 0.5f, this->level->bounds->collisionBox->size.height * 0.5f);

    auto healthBarPosition = barBasePosition + cocos2d::Point(16, -16);
    auto weaponBarPosition = barBasePosition + cocos2d::Point(8, -16);
    auto bossHealthBarPosition = bossBarBasePosition + cocos2d::Point(-16, -16);

    this->healthBar = dynamic_cast<EnergyBar*>(windy::EntityFactory::getInstance().create("energy_bar", healthBarPosition, cocos2d::Size(16, 16)));
    this->weaponBar = dynamic_cast<EnergyBar*>(windy::EntityFactory::getInstance().create("energy_bar", weaponBarPosition, cocos2d::Size(16, 16)));
    this->bossHealthBar = dynamic_cast<EnergyBar*>(windy::EntityFactory::getInstance().create("energy_bar", bossHealthBarPosition, cocos2d::Size(16, 16)));

    this->addChild(this->healthBar);
    this->addChild(this->weaponBar);
    this->addChild(this->bossHealthBar);

    this->healthBar->setValue(-2);
    this->bossHealthBar->setValue(-2);

    this->fillTimer = 0;
    this->fillTimeDelay = 0.06f;

    this->fillTarget = nullptr;

    this->fillAmount = 0;

    this->filling = false;

    this->fillingHealthBar = false;
    this->fillingWeaponBar = false;
    this->fillingBossHealthBar = false;

    this->onRestoreCallback = nullptr;

    this->level->gui = this;
}

void GameGui::restoreHealth(int amount) {
    this->fillTimer = this->fillTimeDelay;

    this->fillAmount = amount;

    this->fillTarget = this->healthBar;

    this->filling = true;

    this->level->setPaused(true);

    this->fillingHealthBar = true;

}

void GameGui::restoreWeapon(int amount) {
    this->fillTimer = this->fillTimeDelay;

    this->fillAmount = amount;

    this->fillTarget = this->weaponBar;

    this->filling = true;

    this->level->setPaused(true);

    this->fillingWeaponBar = true;

}

void GameGui::restoreBossHealth(int amount) {
    this->fillTimer = this->fillTimeDelay;

    this->fillAmount = amount;

    this->fillTarget = this->bossHealthBar;

    this->filling = true;

    this->level->setPaused(true);

    this->fillingBossHealthBar = true;
}

void GameGui::restoreGenericBar(EnergyBar* bar, int amount, std::function<void()> onRestoreCallback) {
    this->fillTimer = this->fillTimeDelay;

    this->fillAmount = amount;

    this->fillTarget = bar;

    this->filling = true;

    this->fillingGenericBar = true;

    this->onRestoreCallback = onRestoreCallback;
}



void GameGui::onUpdate(float dt) {
    if (this->filling) {
        if (this->fillAmount > 0) {
            if (this->fillTimer <= 0) {

                this->fillTimer = 0;

                windy::AudioManager::playSfx(windy::Sounds::GetEnergy);

                int currentValue = this->fillTarget->getValue() + 1;

                this->fillTarget->setValue(currentValue + 1);

                this->fillTimer = this->fillTimeDelay;

                this->fillAmount -= 1;

                if (this->fillTarget->getValue() >= this->fillTarget->getMaxValue()) {
                    this->fillAmount = 0;
                }
            }
            else {
                this->fillTimer -= dt;
            }
        }
        else {
            this->filling = false;
            this->fillTimer = 0;

            if (this->fillTarget == this->healthBar) {
                this->fillingHealthBar = false;
            }
            else if (this->fillTarget == this->weaponBar) {
                this->fillingWeaponBar = false;
            }
            else if (this->fillTarget == this->bossHealthBar) {
                this->fillingBossHealthBar = false;
            }
            else {
                this->fillingGenericBar = false;
                if (this->onRestoreCallback) {
                    this->onRestoreCallback();
                    this->onRestoreCallback = nullptr;
                }
            }

            if (this->fillTarget == this->healthBar || this->fillTarget == this->weaponBar || this->fillTarget == this->bossHealthBar) {
                this->level->setPaused(false);
            }
            
        }
    }
    else {
        this->fillTarget = nullptr;
    }
}