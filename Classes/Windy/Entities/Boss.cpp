#include "Boss.h"

#include "Browner.h"
#include "Ladder.h"
#include "Weapon.h"

#include "./../Armature.h"
#include "./../Sprite.h"
#include "./../GameTags.h"

#include "./../Level.h"
#include "./../PhysicsWorld.h"



using namespace windy;

bool Boss::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->power = 7;

    this->setTag(windy::GameTags::General::Boss);

    this->initVariables();
    this->setupBrowners();

    this->health = 0;

    this->triggerAttack = false;
    this->triggerCharge = false;
    this->triggerDischarge = false;
    this->triggerWalkLeft = false;
    this->triggerWalkRight = false;
    this->triggerJump = false;
    this->triggerStopJump = false;
    this->triggerDashJump = false;
    this->triggerSlide = false;
    this->triggerSlideLeft = false;
    this->triggerSlideRight = false;
    this->triggerClimbUp = false;
    this->triggerClimbDown = false;

    this->introTimer = 0;

    this->state = BossState::Teleporting;


    return true;
}


bool Boss::attackCondition() {
    return this->triggerAttack;
}

bool Boss::chargeCondition() {
    return this->triggerCharge;
}

bool Boss::dischargeCondition() {
    return this->triggerDischarge;
}


bool Boss::walkLeftCondition() {
    return this->triggerWalkLeft;
}

bool Boss::walkRightCondition() {
    return this->triggerWalkRight;
}

bool Boss::jumpCondition() {
    return this->triggerJump;
}

bool Boss::stopJumpCondition() {
    return this->triggerStopJump;
}

bool Boss::dashJumpCondition() {
    return this->triggerDashJump;
}

bool Boss::slideCondition() {
    return this->triggerSlide;
}

bool Boss::slideLeftCondition() {
    return this->triggerSlideLeft;
}

bool Boss::slideRightCondition() {
    return this->triggerSlideRight;
}

bool Boss::climbUpCondition() {
    return this->triggerClimbUp;
}

bool Boss::climbDownCondition() {
    return this->triggerClimbDown;
}


void Boss::onCollisionEnter(Logical* collision) {

    if (collision->getTag() == GameTags::General::Ladder) {
        this->activeLadder = dynamic_cast<Ladder*>(collision);
    }
    else if (collision->getTag() == GameTags::General::Teleporter) {

        if (this->spawning) {
            this->spawning = false;

            this->ignoreGravity = false;
            this->ignoreLandscapeCollision = false;

            this->canMove = true;
            this->onGround = true;

            this->speed = cocos2d::Point(0, 0);

            this->contacts[CollisionContact::Down] = true;

            this->level->physicsWorld->alignCollisions(this, collision, false);


            AudioManager::playSfx(windy::Sounds::Teleport1);

            this->state = BossState::Intro;
        }
    }
}

void Boss::onCollision(Logical* collision) {

    if (collision->getTag() == GameTags::Weapon::WeaponPlayer) {
        auto weapon = dynamic_cast<Weapon*>(collision);
        this->stun(weapon->power);
    }

}



void Boss::onCollisionExit(Logical* collision) {

    if (collision->getTag() == GameTags::General::Ladder) {
        this->activeLadder = nullptr;
    }

}

void Boss::checkHealth() {
    if (this->health <= 0 && this->alive && this->state == BossState::Fighting) {

        this->state = BossState::Defeated;

        this->stopAllActions();

        this->currentBrowner->deactivate();

        this->health = 0;

        this->alive = false;
        this->vulnerable = false;

        this->speed.x = 0;
        this->speed.y = 0;

        AudioManager::stopAll();

        AudioManager::playSfx(windy::Sounds::Death);


        this->kill(true);
    }
}

void Boss::onUpdate(float dt) {
    Player::onUpdate(dt);

    switch (this->state) {
        case BossState::Intro:
            this->switchToBattleBrowner();

            this->introTimer = this->currentBrowner->getActionDuration("intro");

            this->atIntro = true;
            this->state = BossState::Spawning;
            break;

        case BossState::Spawning:
            if (this->introTimer <= 0) {
                this->introTimer = 0;
                this->atIntro = false;
                this->onSpawn();

                this->restoreHealth(28);

                this->state = BossState::RestoringHealth;
            }
            else {
                this->introTimer -= dt;
            }
            break;

        case BossState::RestoringHealth:
            if (this->health >= this->maxHealth) {
                this->state = BossState::Fighting;
            }
            break;

    }

    

    
}