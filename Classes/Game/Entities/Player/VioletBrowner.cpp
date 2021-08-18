#include "VioletBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"


#include "Game/GameManager.h"

#include "Game/Entities/Weapons/VioletBullet.h"

using namespace game;

void VioletBrowner::initConstraints() {
    this->canWalk = true;
    this->canJump = true;
    this->canAttack = true;
    this->canWalkShoot = true;
    this->canJumpShoot = true;
    this->canClimb = true;
    this->canCharge = true;
    this->canSlide = true;
    this->canDashJump = true;
    this->canMorph = true;
    this->hasIntro = false;
    this->canBeHurt = true;

}


void VioletBrowner::setBaseName() {
    this->baseName = "violet";
}

void VioletBrowner::loadActions() {

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",        "violet_stand",         true,   0.10f),
        windy::AnimationAction("slide",        "violet_slide",         true,   0.10f),
        windy::AnimationAction("climb",        "violet_climb",         true,   0.16f),
        windy::AnimationAction("jump",         "violet_jump",          true,   0.10f),
        windy::AnimationAction("dashjump",     "violet_dashjump",      true,   0.10f),
        windy::AnimationAction("walk",         "violet_walk",          true,   0.12f),
        windy::AnimationAction("standshoot",   "violet_standshoot",    true,   0.10f),
        windy::AnimationAction("walkshoot",    "violet_walkshoot",     true,   0.12f),
        windy::AnimationAction("jumpshoot",    "violet_jumpshoot",     true,   0.10f),
        windy::AnimationAction("climbshoot",   "violet_climbshoot",    true,   0.10f),
        windy::AnimationAction("hurt",         "violet_hurt",          true,   0.02f),
        windy::AnimationAction("morph",        "violet_morph",         true,   0.10f)
    };

    this->player->sprite->appendActionSet(actionSet, true, this->baseName);

    this->brownerId = game::GameManager::getInstance().browners.violet->id;
}

void VioletBrowner::spawn() {
    this->energy = -2;
}

void VioletBrowner::restoreWeaponEnergy(int amount) {
    this->energy = -2;
}

void VioletBrowner::fire() {

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