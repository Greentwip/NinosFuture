#include "SheriffBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"


#include "Game/GameManager.h"

#include "Game/Entities/Weapons/VioletBullet.h"

using namespace game;

void SheriffBrowner::initConstraints() {
    this->canWalk = true;
    this->canJump = true;
    this->canAttack = true;
    this->canClimb = true;
    this->canCharge = false;
    this->canSlide = false;
    this->canDashJump = false;
}

void SheriffBrowner::setBaseName() {
    this->baseName = "sheriff";
}

void SheriffBrowner::loadActions() {

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("intro",         "sheriff_intro",         false,   0.10f),
        windy::AnimationAction("stand",         "sheriff_stand",         false,   0.10f),
        windy::AnimationAction("jump",          "sheriff_jump",          false,   0.10f),
        windy::AnimationAction("walk",          "sheriff_walk",          true,    0.12f),
        windy::AnimationAction("standshoot",    "sheriff_standshoot",    false,   0.10f),
        windy::AnimationAction("climb",         "sheriff_climb",         true,    0.16f),
        windy::AnimationAction("hurt",          "sheriff_hurt",          false,   0.02f)
    };

    this->player->sprite->appendActionSet(actionSet, true, this->baseName);

    this->brownerId = game::GameManager::getInstance().browners.sheriff->id;
}

void SheriffBrowner::spawn() {
    this->energy = -1;
}


void SheriffBrowner::fire() {

    int bulletOffset = 12;
    int bulletPower = 1;

    if (this->chargePower.compare("high") == 0) {
        bulletOffset = 26;
        bulletPower = 3;
        windy::AudioManager::playSfx(windy::Sounds::BusterHigh);
    }
    else if (this->chargePower.compare("mid") == 0) {
        bulletPower = 2;
        windy::AudioManager::playSfx(windy::Sounds::BusterMid);
    }
    else {
        windy::AudioManager::playSfx(windy::Sounds::BusterLow);
    }

    auto bulletPosition = cocos2d::Point(this->player->getPositionX() + (bulletOffset * this->getSpriteNormal()),
        this->player->getPositionY() - 4);


    auto chargePower = this->chargePower;

    auto entryCollisionBox = VioletBullet::getEntryCollisionRectangle(chargePower, bulletPosition, cocos2d::Size(16, 16));
    auto entry = windy::Logical::getEntry(entryCollisionBox, [=]() {
        auto bullet = VioletBullet::create();
        bullet->setPosition(bulletPosition);
        bullet->setup(chargePower);

        bullet->fire(bulletPower, this->getSpriteNormal(), windy::GameTags::WeaponPlayer);

        return bullet;
        });

    this->level->objectManager->objectEntries.push_back(entry);


    //self:getParent():getParent().bullets_[bullet] = bullet

}