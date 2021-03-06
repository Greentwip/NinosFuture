#include "VineBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"


#include "Game/GameManager.h"

#include "Game/Entities/Weapons/VineBall.h"

using namespace game;

void VineBrowner::initConstraints() {
    this->canWalk = true;
    this->canJump = true;
    this->canAttack = true;
    this->canWalkShoot = false;
    this->canJumpShoot = false;
    this->canClimb = true;
    this->canCharge = false;
    this->canSlide = true;
    this->canDashJump = false;
    this->canMorph = false;
    this->hasIntro = true;
    this->canBeHurt = true;

}

void VineBrowner::setBaseName() {
    this->baseName = "vine";
}

void VineBrowner::loadActions() {

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("intro",         "vine_intro",         false,   0.10f),
        windy::AnimationAction("stand",         "vine_stand",         false,   0.10f),
        windy::AnimationAction("jump",          "vine_jump",          false,   0.10f),
        windy::AnimationAction("walk",          "vine_walk",          true,    0.12f),
        windy::AnimationAction("standshoot",    "vine_standshoot",    false,   0.10f),
        windy::AnimationAction("climb",         "vine_climb",         true,    0.16f),
        windy::AnimationAction("slide",         "vine_slide",         true,    0.10f),
        windy::AnimationAction("hurt",          "vine_hurt",          false,   0.02f)
    };

    this->player->sprite->appendActionSet(actionSet, true, this->baseName);

    this->brownerId = game::GameManager::getInstance().browners.vine->id;
}

void VineBrowner::spawn() {
    this->energy = 28;
}

void VineBrowner::attack() {

    if (this->player->attackCondition() &&
        !this->player->jumping &&
        !this->player->walking &&
        !this->player->stunned &&
        !this->player->attacking) {

        if (this->energy > 0) {
            this->energy -= 1;

            this->player->attacking = true;

            auto preDelay = cocos2d::DelayTime::create(this->getActionDuration("standshoot") * 0.45f);
            auto preCallback = cocos2d::CallFunc::create([this]() { this->fire(); });

            //auto postDelay = cocos2d::DelayTime::create(this->getActionDuration("standshoot") * 0.5f);

            auto postCallback = cocos2d::CallFunc::create([this]() { this->player->attacking = false; });

            auto sequence = cocos2d::Sequence::create(preDelay, preCallback, postCallback, nullptr);

            ((cocos2d::Node*)this)->runAction(sequence);
        }

    }

}
void VineBrowner::fire() {

    int bulletOffset = 24;
    int bulletPower = 3;

    windy::AudioManager::playSfx(windy::Sounds::BusterMid);

    auto bulletPositionA = cocos2d::Point(
        this->player->getPositionX(),
        this->player->getPositionY() + 32);

    auto entryCollisionBoxA = VineBall::getEntryCollisionRectangle(bulletPositionA, cocos2d::Size(16, 16));
    auto entryA = windy::Logical::getEntry(entryCollisionBoxA, [=]() {
        auto bullet = VineBall::create();
        bullet->setPosition(bulletPositionA);
        bullet->setup();

        bullet->fire(bulletPower, this->getSpriteNormal(), this->player->weaponTag);

        return bullet;
    }, 512);

    this->level->objectManager->objectEntries.push_back(entryA);

}