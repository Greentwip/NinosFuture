#include "MilitaryMan.h"

#include "Windy/Entities/Browner.h"
#include "Windy/Entities/Bounds.h"

#include "Windy/Sprite.h"

#include "Game/GameManager.h"

using namespace game;

void MilitaryMan::switchToBattleBrowner() {
    this->switchBrowner(GameManager::getInstance().browners.military->id);
}

void MilitaryMan::onSpawn() {
    this->state = BossState::Fighting;

    this->jumpCounter = 0;
    this->jumpAmount = 1;
}

void MilitaryMan::attack() {
    this->attacking = true;

    auto preDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot"));
    auto preCallback = cocos2d::CallFunc::create([this]() { this->currentBrowner->fire(); });

    auto postDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 0.5f);

    auto postCallback = cocos2d::CallFunc::create([this]() { this->attacking = false; });

    auto sequence = cocos2d::Sequence::create(preDelay, preCallback, postDelay, postCallback, nullptr);

    this->runAction(sequence);
}

void MilitaryMan::onUpdate(float dt) {
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

                auto shootDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 4);

                auto stateCallback = cocos2d::CallFunc::create([this]() {
                    this->jumpCounter = 0;
                    this->attackState = AttackState::DashingLeft1;
                });

                auto sequence = cocos2d::Sequence::create(shootAttack, shootDelay, shootAttack, shootDelay, stateCallback, nullptr);

                this->attackState = AttackState::Idle;

                this->runAction(sequence);

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

                auto shootDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 4);

                auto stateCallback = cocos2d::CallFunc::create([this]() {
                    this->jumpCounter = 0;
                    this->attackState = AttackState::DashingRight1;
                    });

                auto sequence = cocos2d::Sequence::create(shootAttack, shootDelay, shootAttack, shootDelay, stateCallback, nullptr);

                this->attackState = AttackState::Idle;

                this->runAction(sequence);

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
            }
            break;

            case AttackState::Idle:
            {
            
            }
            break;

        }


    }

}

