#include "VineMan.h"

#include "Windy/Entities/Browner.h"
#include "Windy/Entities/Bounds.h"

#include "Windy/Sprite.h"

#include "Game/GameManager.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"
#include "Game/Entities/Weapons/VineBall.h"


using namespace game;

void VineMan::switchToBattleBrowner() {
    this->switchBrowner(GameManager::getInstance().browners.vine->id);
}

void VineMan::onSpawn() {
    this->state = BossState::Fighting;

    this->jumpCounter = 0;
    this->jumpAmount = 1;
    this->attackCounter = 0;
    this->_isFullDash = false;
}

void VineMan::fire(const cocos2d::Point& weaponSpeed) {
    
    int bulletOffset = 24;
    int bulletPower = 3;

    windy::AudioManager::playSfx(windy::Sounds::BusterMid);

    auto bulletPositionA = cocos2d::Point(
        this->getPositionX(),
        this->getPositionY() + 32);

    auto entryCollisionBoxA = VineBall::getEntryCollisionRectangle(bulletPositionA, cocos2d::Size(16, 16));
    auto entryA = windy::Logical::getEntry(entryCollisionBoxA, [=]() {
        auto bullet = VineBall::create();
        bullet->setPosition(bulletPositionA);
        bullet->setup();

        bullet->fire(bulletPower, this->currentBrowner->getSpriteNormal(), this->weaponTag, weaponSpeed);

        return bullet;
        });

    this->level->objectManager->objectEntries.push_back(entryA);
}

void VineMan::attack() {
    this->attacking = true;

    auto preDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 0.45f);

    auto preCallback = cocos2d::CallFunc::create([this]() { 
        
        if (this->attackCounter == 0) {
            auto weaponSpeed = cocos2d::Point(1.65f, 4.0f);
            this->fire(weaponSpeed);
        }
        else if (this->attackCounter == 1) {
            auto weaponSpeed = cocos2d::Point(2.25f, 6.0f);
            this->fire(weaponSpeed);
        }
        else if (this->attackCounter == 2) {
            auto weaponSpeed = cocos2d::Point(4.0f, 6.0f);
            this->fire(weaponSpeed);
        }
        else {
            this->fire();
        }

        this->attackCounter += 1;
        
    });

    auto postCallback = cocos2d::CallFunc::create([this]() { this->attacking = false; });

    auto sequence = cocos2d::Sequence::create(preDelay, preCallback, postCallback, nullptr);

    this->runAction(sequence);
}

void VineMan::onUpdate(float dt) {
    GameBoss::onUpdate(dt);

    if (this->state == BossState::Fighting) {

        switch (this->attackState)
        {
            case AttackState::ShootingLeft1:
            {
                this->speed.x = 0;

                this->sprite->setFlippedX(true);

                auto shootAttack = cocos2d::CallFunc::create([this]() {
                    this->attack();
                });

                auto shootDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 1.5f);

                auto stateCallback = cocos2d::CallFunc::create([this]() {
                    this->jumpCounter = 0;
                    this->attackCounter = 0;

                    this->attackState = this->_isFullDash ? AttackState::FullDashLeft : AttackState::DashingLeft1;
                });

                auto sequence = cocos2d::Sequence::create(
                    shootAttack, 
                    shootDelay, 
                    shootAttack, 
                    shootDelay, 
                    shootAttack,
                    shootDelay,
                    stateCallback, 
                    nullptr);

                this->attackState = AttackState::Idle;

                this->runAction(sequence);

            }
            break;

            case AttackState::FullDashLeft:
            {
                this->sprite->setFlippedX(true);

                this->speed.x = this->walkSpeed * -1.5f;

                if (this->contacts[windy::CollisionContact::Left]) {
                    this->speed.x = 0;
                    this->attackState = AttackState::ShootingRight1;
                }

            }
            break;

            case AttackState::DashingLeft1:
            {
                this->sprite->setFlippedX(true);

                this->speed.x = this->walkSpeed * -1.5f;

                if (this->getPositionX() <= this->level->bounds->getPositionX() + 32) {
                    this->attackState = AttackState::JumpingLeft;
                }
            }
            break;

            case AttackState::JumpingLeft:
            {
                this->sprite->setFlippedX(true);

                if (this->contacts[windy::CollisionContact::Down]) {
                    if (this->jumpCounter < this->jumpAmount) {
                        this->jumpCounter++;

                        this->speed.x = this->walkSpeed * -1.25f;
                        this->speed.y = this->jumpSpeed * 1.25f;
                    }
                    else {
                        this->attackState = AttackState::DashingLeft2;
                    }
                }
            }
            break;

            case AttackState::DashingLeft2:
            {
                this->sprite->setFlippedX(true);

                this->speed.x = this->walkSpeed * -1.5f;

                if (this->contacts[windy::CollisionContact::Left]) {
                    this->speed.x = 0;
                    this->attackState = AttackState::ShootingRight1;
                }
            }
            break;

            case AttackState::ShootingRight1:
            {
                this->speed.x = 0;

                this->sprite->setFlippedX(false);

                auto shootAttack = cocos2d::CallFunc::create([this]() {
                    this->attack();
                    });

                auto shootDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 1.5f);

                auto stateCallback = cocos2d::CallFunc::create([this]() {
                    this->jumpCounter = 0;
                    this->attackCounter = 0;

                    this->attackState = this->_isFullDash ? AttackState::FullDashRight : AttackState::DashingRight1;
                    });

                auto sequence = cocos2d::Sequence::create(
                    shootAttack,
                    shootDelay,
                    shootAttack,
                    shootDelay,
                    shootAttack,
                    shootDelay,
                    stateCallback,
                    nullptr);

                this->attackState = AttackState::Idle;

                this->runAction(sequence);

            }
            break;

            case AttackState::FullDashRight: 
            {
                this->sprite->setFlippedX(false);

                this->speed.x = this->walkSpeed * 1.5f;

                if (this->contacts[windy::CollisionContact::Right]) {
                    this->speed.x = 0;
                    this->attackState = AttackState::CoolDown;
                }

            }
            break;

            case AttackState::DashingRight1:
            {
                this->sprite->setFlippedX(false);

                this->speed.x = this->walkSpeed * 1.5f;

                if (this->getPositionX() >= this->level->bounds->getPositionX() - 32) {
                    this->attackState = AttackState::JumpingRight;
                }
            }
            break;

            case AttackState::JumpingRight:
            {
                this->sprite->setFlippedX(false);

                if (this->contacts[windy::CollisionContact::Down]) {
                    if (this->jumpCounter < this->jumpAmount) {
                        this->jumpCounter++;

                        this->speed.x = this->walkSpeed * 1.25f;
                        this->speed.y = this->jumpSpeed * 1.25f;
                    }
                    else {
                        this->attackState = AttackState::DashingRight2;
                    }
                }
            }
            break;

            case AttackState::DashingRight2:
            {
                this->sprite->setFlippedX(false);

                this->speed.x = this->walkSpeed * 1.5f;

                if (this->contacts[windy::CollisionContact::Right]) {
                    this->speed.x = 0;
                    this->attackState = AttackState::CoolDown;
                }
            }
            break;

            case AttackState::CoolDown:
            {
                this->sprite->setFlippedX(false);

                this->attackState = AttackState::ShootingLeft1;

                this->_isFullDash = !this->_isFullDash;
            }
            break;

            case AttackState::Idle:
            {
            
            }
            break;

        }


    }

}

