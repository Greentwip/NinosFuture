#include "SheriffMan.h"

#include "Windy/Entities/Browner.h"

#include "Windy/Sprite.h"

#include "Game/GameManager.h"

using namespace game;

void SheriffMan::switchToBattleBrowner() {
    this->switchBrowner(GameManager::getInstance().browners.sheriff->id);
}

void SheriffMan::onSpawn() {
    this->state = BossState::Fighting;

    this->jumpCounter = 0;
    this->jumpAmount = 1;
}

void SheriffMan::attack() {
    this->attacking = true;

    auto preDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot"));
    auto preCallback = cocos2d::CallFunc::create([this]() { this->currentBrowner->fire(); });

    auto postDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 0.5f);

    auto postCallback = cocos2d::CallFunc::create([this]() { this->attacking = false; });

    auto sequence = cocos2d::Sequence::create(preDelay, preCallback, postDelay, postCallback, nullptr);

    this->runAction(sequence);
}

void SheriffMan::onUpdate(float dt) {
    GameBoss::onUpdate(dt);

    if (this->state == BossState::Fighting) {

        switch (this->attackState) 
        {
            case AttackState::ShootingLeft: 
            {
                this->speed.x = 0;

                this->sprite->setFlippedX(true);

                auto shootAttack = cocos2d::CallFunc::create([this]() {
                    this->attack();
                });

                auto shootDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 4);

                auto stateCallback = cocos2d::CallFunc::create([this]() { 
                    this->jumpCounter = 0;
                    this->attackState = AttackState::RammingLeft; 
                });

                auto sequence = cocos2d::Sequence::create(shootAttack, shootDelay, shootAttack, shootDelay, stateCallback, nullptr);
                
                this->attackState = AttackState::Idle;

                this->runAction(sequence);

            }
            break;

            case AttackState::RammingLeft:
            {
                this->sprite->setFlippedX(true);

                this->speed.x = this->walkSpeed * -1.5f;

                auto playerPosition = this->level->player->getPosition();
                auto bossPosition = this->getPosition();
                
                float playerDistance = bossPosition.getDistance(playerPosition);


                if (playerDistance < 48) {
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
                        this->attackState = AttackState::DashingLeft;
                    }
                }
            }
            break;

            case AttackState::DashingLeft:
            {
                this->sprite->setFlippedX(true);

                this->speed.x = this->walkSpeed * -1.5f;

                if (this->contacts[windy::CollisionContact::Left]) {
                    this->speed.x = 0;
                    this->attackState = AttackState::ShootingRight;
                }
            }
            break;


            case AttackState::ShootingRight:
            {
                this->speed.x = 0;

                this->sprite->setFlippedX(false);

                auto shootAttack = cocos2d::CallFunc::create([this]() {
                    this->attack();
                });

                auto shootDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 4);

                auto stateCallback = cocos2d::CallFunc::create([this]() {
                    this->jumpCounter = 0;
                    this->attackState = AttackState::RammingRight;
                });

                auto sequence = cocos2d::Sequence::create(shootAttack, shootDelay, shootAttack, shootDelay, stateCallback, nullptr);

                this->attackState = AttackState::Idle;

                this->runAction(sequence);

            }
            break;

            case AttackState::RammingRight:
            {
                this->sprite->setFlippedX(false);

                this->speed.x = this->walkSpeed * 1.5f;

                auto playerPosition = this->level->player->getPosition();
                auto bossPosition = this->getPosition();

                float playerDistance = bossPosition.getDistance(playerPosition);


                if (playerDistance < 48) {
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
                        this->speed.y = this->jumpSpeed * 1.5f;
                    }
                    else {
                        this->attackState = AttackState::DashingRight;
                    }
                }
            }
            break;

            case AttackState::DashingRight:
            {
                this->sprite->setFlippedX(false);

                this->speed.x = this->walkSpeed * 1.5f;

                if (this->contacts[windy::CollisionContact::Right]) {
                    this->speed.x = 0;
                    this->attackState = AttackState::ChargingLeft;
                }
            }
            break;

            case AttackState::ChargingLeft:
            case AttackState::ChargingRight: 
            {
                this->attackState = AttackState::ShootingLeft;
            }
            break;

        }


    }
    
}

