#include "NightBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"


#include "Game/GameManager.h"

#include "Game/Entities/Weapons/Shiki108.h"

using namespace game;

void NightBrowner::initConstraints() {
    this->canWalk = true;
    this->canJump = true;
    this->canAttack = true;
    this->canWalkShoot = false;
    this->canJumpShoot = false;
    this->canClimb = true;
    this->canCharge = false;
    this->canSlide = false;
    this->canDashJump = false;
    this->canMorph = false;
    this->hasIntro = true;
    this->canBeHurt = true;

}

void NightBrowner::setBaseName() {
    this->baseName = "night";
}

void NightBrowner::loadActions() {

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("intro",         "night_intro",         false,   0.10f),
        windy::AnimationAction("stand",         "night_stand",         false,   0.10f),
        windy::AnimationAction("jump",          "night_jump",          false,   0.10f),
        windy::AnimationAction("walk",          "night_walk",          true,    0.12f),
        windy::AnimationAction("standshoot",    "night_standshoot",    false,   0.05f),
        windy::AnimationAction("climb",         "night_climb",         true,    0.16f),
        windy::AnimationAction("hurt",          "night_hurt",          false,   0.02f)
    };

    this->player->sprite->appendActionSet(actionSet, true, this->baseName);

    this->brownerId = game::GameManager::getInstance().browners.night->id;
}

void NightBrowner::spawn() {
    this->energy = 28;
}

void NightBrowner::attack() {

    if (this->player->attackCondition() &&
        !this->player->jumping &&
        !this->player->walking &&
        !this->player->stunned &&
        !this->player->attacking) {

        if (this->energy > 0) {
            this->energy -= 1;

            this->player->attacking = true;

            auto preDelay = cocos2d::DelayTime::create(this->getActionDuration("standshoot") * 0.8f);
            auto preCallback = cocos2d::CallFunc::create([this]() { this->fire(); });

            auto postDelay = cocos2d::DelayTime::create(this->getActionDuration("standshoot") * 0.2f);

            auto postCallback = cocos2d::CallFunc::create([this]() { this->player->attacking = false; });

            auto sequence = cocos2d::Sequence::create(preDelay, preCallback, postDelay, postCallback, nullptr);

            ((cocos2d::Node*)this)->runAction(sequence);
        }

    }

}
void NightBrowner::fire() {

    int bulletOffset = 24;
    int bulletPower = 3;

    windy::AudioManager::playSfx(windy::Sounds::BusterMid);

    auto bulletPositionA = cocos2d::Point(
        this->player->getPositionX() + (bulletOffset * this->getSpriteNormal()),
        this->player->getPositionY() - 2);

    auto entryCollisionBoxA = Shiki108::getEntryCollisionRectangle(bulletPositionA, cocos2d::Size(16, 16));
    auto entryA = windy::Logical::getEntry(entryCollisionBoxA, [=]() {
        auto bullet = Shiki108::create();
        bullet->setPosition(bulletPositionA);
        bullet->setup();

        bullet->fire(bulletPower, this->getSpriteNormal(), this->player->weaponTag);

        return bullet;
        });

    this->level->objectManager->objectEntries.push_back(entryA);

}