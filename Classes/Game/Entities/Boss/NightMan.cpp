#include "NightMan.h"

#include "Windy/Entities/Browner.h"
#include "Windy/Entities/Bounds.h"

#include "Windy/Sprite.h"

#include "Game/GameManager.h"

#include "Windy/ObjectManager.h"
#include "Windy/Level.h"
#include "Game/Entities/Weapons/VineBall.h"


using namespace game;

void NightMan::switchToBattleBrowner() {
    this->switchBrowner(GameManager::getInstance().browners.night->id);
}

void NightMan::onSpawn() {
    this->state = BossState::Fighting;

    this->jumpCounter = 0;
    this->jumpAmount = 1;
    this->attackCounter = 0;
    this->_attackSubState = AttackSubState::Dash;
}

void NightMan::attack() {
    this->attacking = true;

    auto preDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 0.8f);
    
    auto preCallback = cocos2d::CallFunc::create([this]() { this->currentBrowner->fire();});

    auto postDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot") * 0.2f);

    auto postCallback = cocos2d::CallFunc::create([this]() { this->attacking = false; });

    auto sequence = cocos2d::Sequence::create(preDelay, preCallback, postCallback, nullptr);

    this->runAction(sequence);
}

void NightMan::onUpdate(float dt) {
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

                auto shootDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot"));

                auto stateCallback = cocos2d::CallFunc::create([this]() {
                    this->jumpCounter = 0;

                    switch (_attackSubState) {
                        case Dash:
                        {
                            this->attackState = AttackState::FullDashLeft;
                        }
                        break;

                        case Jump:
                        {
                            this->attackState = AttackState::DashingLeft1;
                        }
                        break;

                        case Ram:
                        {
                            this->attackState = AttackState::RammingLeft;
                        }
                        break;
                    }
                    
                });

                if (this->health <= 14) {
                    auto sequence = cocos2d::Sequence::create(
                        shootAttack,
                        shootDelay,
                        shootAttack,
                        shootDelay,
                        stateCallback,
                        nullptr);

                    this->attackState = AttackState::Idle;

                    this->runAction(sequence);
                }
                else {
                    auto sequence = cocos2d::Sequence::create(
                        shootAttack,
                        shootDelay,
                        stateCallback,
                        nullptr);

                    this->attackState = AttackState::Idle;

                    this->runAction(sequence);
                }

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

                auto shootDelay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("standshoot"));

                auto stateCallback = cocos2d::CallFunc::create([this]() {
                    this->jumpCounter = 0;
                    switch (_attackSubState) {
                        case Dash:
                        {
                            this->attackState = AttackState::FullDashRight;
                        }
                        break;

                        case Jump:
                        {
                            this->attackState = AttackState::DashingRight1;
                        }
                        break;

                        case Ram:
                        {
                            this->attackState = AttackState::RammingRight;
                        }
                        break;
                    }
                });

                if (this->health <= 14) {
                    auto sequence = cocos2d::Sequence::create(
                        shootAttack,
                        shootDelay,
                        shootAttack,
                        shootDelay,
                        stateCallback,
                        nullptr);

                    this->attackState = AttackState::Idle;

                    this->runAction(sequence);
                }
                else {
                    auto sequence = cocos2d::Sequence::create(
                        shootAttack,
                        shootDelay,
                        stateCallback,
                        nullptr);

                    this->attackState = AttackState::Idle;

                    this->runAction(sequence);
                }
                

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

                if (this->_attackSubState == AttackSubState::Dash) {
                    this->_attackSubState = AttackSubState::Jump;
                }
                else if (this->_attackSubState == AttackSubState::Jump) {
                    this->_attackSubState = AttackSubState::Ram;
                }

                if (this->_attackSubState == AttackSubState::Ram) {
                    this->_attackSubState = AttackSubState::Dash;
                }
            }
            break;

            case AttackState::Idle:
            {
            
            }
            break;

        }


    }

}

