#include "SheriffBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"


#include "Game/GameManager.h"

#include "Game/Entities/Weapons/SheriffBullet.h"

using namespace game;

void SheriffBrowner::initConstraints() {
    this->canWalk = true;
    this->canJump = true;
    this->canAttack = true;
    this->canWalkShoot = false;
    this->canJumpShoot = false;
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
    this->energy = 28;
}

void SheriffBrowner::attack() {

    if (this->player->attackCondition() && 
        !this->player->jumping && 
        !this->player->walking && 
        !this->player->stunned && 
        !this->player->attacking) {

        if (this->energy > 0) {
            this->energy -= 1;

            this->player->attacking = true;

            auto preDelay = cocos2d::DelayTime::create(this->getActionDuration("standshoot"));
            auto preCallback = cocos2d::CallFunc::create([this]() { this->fire(); });

            auto postDelay = cocos2d::DelayTime::create(this->getActionDuration("standshoot") * 0.5f);

            auto postCallback = cocos2d::CallFunc::create([this]() { this->player->attacking = false; });

            auto sequence = cocos2d::Sequence::create(preDelay, preCallback, postDelay, postCallback, nullptr);

            ((cocos2d::Node*)this)->runAction(sequence);
        }

    }

}
void SheriffBrowner::fire() {

    int bulletOffset = 24;
    int bulletPower = 3;

    windy::AudioManager::playSfx(windy::Sounds::BusterMid);


    auto bulletPositionA = cocos2d::Point(
        this->player->getPositionX() + (bulletOffset * this->getSpriteNormal()),
        this->player->getPositionY() - 2);

    auto bulletPositionB = cocos2d::Point(
        this->player->getPositionX() - (bulletOffset * this->getSpriteNormal()),
        this->player->getPositionY() - 2);


    auto entryCollisionBoxA = SheriffBullet::getEntryCollisionRectangle(bulletPositionA, cocos2d::Size(16, 16));
    auto entryA = windy::Logical::getEntry(entryCollisionBoxA, [=]() {
        auto bullet = SheriffBullet::create();
        bullet->setPosition(bulletPositionA);
        bullet->setup();

        bullet->fire(bulletPower, this->getSpriteNormal(), this->player->weaponTag);

        return bullet;
    });

    auto entryCollisionBoxB = SheriffBullet::getEntryCollisionRectangle(bulletPositionB, cocos2d::Size(16, 16));
    auto entryB = windy::Logical::getEntry(entryCollisionBoxB, [=]() {
        auto bullet = SheriffBullet::create();
        bullet->setPosition(bulletPositionB);
        bullet->setup();

        bullet->fire(bulletPower, this->getSpriteNormal() * -1, this->player->weaponTag);

        return bullet;
    });


    this->level->objectManager->objectEntries.push_back(entryA);
    this->level->objectManager->objectEntries.push_back(entryB);


    //self:getParent():getParent().bullets_[bullet] = bullet

}