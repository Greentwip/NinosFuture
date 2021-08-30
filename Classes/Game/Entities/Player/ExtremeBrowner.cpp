#include "ExtremeBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"


#include "Game/GameManager.h"

#include "Game/Entities/Weapons/ExtremeBullet.h"

using namespace game;

void ExtremeBrowner::initConstraints() {
    this->canWalk = true;
    this->canJump = true;
    this->canAttack = true;
    this->canWalkShoot = true;
    this->canJumpShoot = true;
    this->canClimb = true;
    this->canCharge = false;
    this->canSlide = false;
    this->canDashJump = true;
    this->canMorph = false;
    this->hasIntro = false;
    this->canBeHurt = true;

}

void ExtremeBrowner::setBaseName() {
    this->baseName = "extreme";
}

void ExtremeBrowner::loadActions() {

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",         "extreme_stand",         true,    0.10f),
        windy::AnimationAction("jump",          "extreme_jump",          true,    0.10f),
        windy::AnimationAction("dashjump",      "extreme_dashjump",      true,    0.10f),
        windy::AnimationAction("walk",          "extreme_walk",          true,    0.12f),
        windy::AnimationAction("standshoot",    "extreme_standshoot",    false,   0.10f),
        windy::AnimationAction("climb",         "extreme_climb",         true,    0.16f),
        windy::AnimationAction("walkshoot",     "extreme_walkshoot",     true,    0.12f),
        windy::AnimationAction("jumpshoot",     "extreme_jumpshoot",     true,    0.10f),
        windy::AnimationAction("hurt",          "extreme_hurt",          false,   0.02f)
    };

    this->player->sprite->appendActionSet(actionSet, true, this->baseName);

    this->brownerId = game::GameManager::getInstance().browners.extreme->id;
}

void ExtremeBrowner::spawn() {
    this->energy = 28;
}

void ExtremeBrowner::attack() {

    if (this->player->attackCondition() &&
        !this->player->stunned &&
        !this->player->attacking) {

        if (this->energy > 0) {
            this->energy -= 1;

            this->player->attacking = true;
            this->timedShoot();
        }

    }

}
void ExtremeBrowner::fire() {

    int bulletOffset = 50;
    int bulletPower = 5;

    windy::AudioManager::playSfx(windy::Sounds::BusterHigh);

    float bulletOffsetY = 4;

    if (this->player->jumping) {
        bulletOffsetY += 4;
    }

    auto bulletPosition = cocos2d::Point(this->player->getPositionX() + (bulletOffset * this->getSpriteNormal()),
        this->player->getPositionY() + bulletOffsetY);

    auto entryCollisionBox = ExtremeBullet::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 16));
    auto entry = windy::Logical::getEntry(entryCollisionBox, [=]() {
        auto bullet = ExtremeBullet::create();
        bullet->setPosition(bulletPosition);
        bullet->setup();

        bullet->fire(bulletPower, this->getSpriteNormal(), this->player->weaponTag);

        return bullet;
    });

    this->level->objectManager->objectEntries.push_back(entry);

}