#include "HelmetBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"


#include "Game/GameManager.h"

#include "Game/Entities/Weapons/VioletBullet.h"

using namespace game;

void HelmetBrowner::initConstraints() {
    this->canWalk = true;
    this->canJump = true;
    this->canAttack = true;
    this->canWalkShoot = true;
    this->canJumpShoot = true;
    this->canClimb = true;
    this->canCharge = true;
    this->canSlide = true;
    this->canDashJump = true;
    this->canMorph = false;
    this->hasIntro = false;
    this->canBeHurt = true;

}


void HelmetBrowner::setBaseName() {
    this->baseName = "helmet";
}

void HelmetBrowner::loadActions() {

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",        "helmet_stand",         true,   0.10f),
        windy::AnimationAction("slide",        "helmet_slide",         true,   0.10f),
        windy::AnimationAction("climb",        "helmet_climb",         true,   0.16f),
        windy::AnimationAction("jump",         "helmet_jump",          true,   0.10f),
        windy::AnimationAction("dashjump",     "helmet_dashjump",      true,   0.10f),
        windy::AnimationAction("walk",         "helmet_walk",          true,   0.12f),
        windy::AnimationAction("standshoot",   "helmet_standshoot",    true,   0.10f),
        windy::AnimationAction("walkshoot",    "helmet_walkshoot",     true,   0.12f),
        windy::AnimationAction("jumpshoot",    "helmet_jumpshoot",     true,   0.10f),
        windy::AnimationAction("climbshoot",   "helmet_climbshoot",    true,   0.10f),
        windy::AnimationAction("hurt",         "helmet_hurt",          true,   0.02f),
    };

    this->player->sprite->appendActionSet(actionSet, true, this->baseName);

    this->brownerId = game::GameManager::getInstance().browners.helmet->id;
}

void HelmetBrowner::spawn() {
    this->energy = -2;
}

void HelmetBrowner::restoreWeaponEnergy(int amount) {
    this->energy = -2;
}


void HelmetBrowner::fire() {

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

        bullet->fire(bulletPower, this->getSpriteNormal(), this->player->weaponTag);

        return bullet;
        });

    this->level->objectManager->objectEntries.push_back(entry);


    //self:getParent():getParent().bullets_[bullet] = bullet

}