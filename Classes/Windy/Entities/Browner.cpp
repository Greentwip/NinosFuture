#include "Browner.h"

#include "Player.h"
#include "../ObjectManager.h"

using namespace windy;

bool Browner::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->brownerId = -1;

    return true;
}

void Browner::initConstraints()
{
    this->canWalk = true;
    this->canJump = true;
    this->canDashJump = true;
    this->canSlide = true;
    this->canClimb = true;
    this->canAttack = true;
    this->canWalkShoot = false;
    this->canJumpShoot = false;
    this->canCharge = true;
}

void Browner::initVariables() 
{
    this->chargeTimer = 0;
    this->attackTimer = 0;

    this->maxEnergy = 28;
    this->energy = this->maxEnergy;

    this->chargePower = ChargePower::low;

    this->tintA = false;
    this->tintB = false;

    this->chargeFxColor = 0;

    this->spriteColor = this->player->sprite->getColor();
}

void Browner::setBaseName() {
    this->baseName = "violet";
}

void Browner::spawn() {
    this->energy = 28;
}

void Browner::restoreWeaponEnergy(int amount) {
    this->energy += amount;

    if (this->energy >= this->maxEnergy) {
        this->energy = this->maxEnergy;
    }
}

void Browner::activate() {
    this->player->sprite->setVisible(true);
    this->player->sprite->setColor(this->spriteColor);
}

void Browner::deactivate() {
    this->stopActions();
    this->stopAllActions();
    this->player->sprite->setVisible(false);
    this->tintA = false;
    this->tintB = false;
    this->player->charging = false;
    this->chargeTimer = 0;
    this->chargePower = ChargePower::low;
}

void Browner::runAction(const std::string& action) {
    this->player->sprite->setAnimation(action + "/" + this->baseName + "_" + action);
    this->player->sprite->runAction(action, this->baseName);
}

float Browner::getActionDuration(const std::string& action) {
    return this->player->sprite->getActionDuration(action, this->baseName);
}

int Browner::getCurrentAnimationNumberOfFrames() {
    return this->player->sprite->getCurrentAnimationNumberOfFrames();
}

void Browner::stopActions() {
    this->player->sprite->stopActions();
}

void Browner::stopAllActions() {
    this->player->sprite->stopAllActions();
}


void Browner::pauseActions() {
    this->player->sprite->pauseActions();
}

void Browner::resumeActions() {
    this->player->sprite->resumeActions();
}

void Browner::increaseOrLoopFrame() {
    this->player->sprite->increaseOrLoopFrame();
}

int Browner::getSpriteNormal() {
    
    if (this->player->sprite->isFlippedX()) {
        return -1;
    }
    else {
        return 1;
    }
}

void Browner::charge() {

    if (this->player->charging) {
        this->chargeTimer += 1;

        if (this->chargeTimer == 20) {
            this->chargePower = ChargePower::mid;
            AudioManager::playSfx(Sounds::BusterChargingMid);

            if (!this->tintA) {
                auto colorA = cocos2d::Color3B(188, 188, 188);
                auto colorB = cocos2d::Color3B(220, 40, 0);

                auto tintToA = cocos2d::TintTo::create(0.01f, colorA);
                auto tintToB = cocos2d::TintTo::create(0.01f, colorB);
                auto tintToD = cocos2d::TintTo::create(0.01f, this->spriteColor);

                auto sequence = cocos2d::Sequence::create(tintToA, tintToD, tintToB, tintToD, nullptr);
                auto forever = cocos2d::RepeatForever::create(sequence);
                forever->setTag(GameTags::Actions::Color);
                this->player->sprite->stopAllActionsByTag(GameTags::Actions::Color);
                ((Node*)this->player->sprite)->runAction(forever);
                this->tintA = true;
            }
        }
        else if (this->chargeTimer >= 60) {
            if (!this->tintB) {
                auto colorA = cocos2d::Color3B(188, 188, 188);
                auto colorB = cocos2d::Color3B(220, 40, 0);

                auto tintToA = cocos2d::TintTo::create(0.005f, colorA);
                auto tintToB = cocos2d::TintTo::create(0.005f, colorB);
                auto tintToD = cocos2d::TintTo::create(0.005f, this->spriteColor);

                auto sequence = cocos2d::Sequence::create(tintToA, tintToD, tintToB, tintToD, nullptr);
                auto forever = cocos2d::RepeatForever::create(sequence);
                forever->setTag(GameTags::Actions::Color);
                this->player->sprite->stopAllActionsByTag(GameTags::Actions::Color);
                ((Node*)this->player->sprite)->runAction(forever);
                this->tintB = true;

            }

            if (this->chargeTimer % 18 == 0) {
                AudioManager::playSfx(Sounds::BusterChargingHigh);
                this->chargeTimer = 60;
            }

            this->chargePower = ChargePower::high;

        }
    }
    else {
        this->chargeTimer = 0;
        this->tintA = false;
        this->tintB = false;
        this->player->sprite->stopAllActionsByTag(GameTags::Actions::Color);
        this->player->sprite->setColor(this->spriteColor);
    }
}

void Browner::timedShoot() {
    this->fire();

    if (!this->player->attacking) {
        this->player->attacking = true;

        this->player->dashJumping = false;

        auto delay = cocos2d::DelayTime::create(0.280f);
        auto callback = cocos2d::CallFunc::create([this]() {
            this->player->charging = false;
            this->player->attacking = false;
            this->chargeTimer = 0;
        });

        auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);

        ((Node*)this)->runAction(sequence);
    }
    else {
        this->player->dashJumping = false;

        auto delay = cocos2d::DelayTime::create(0.280f);
        auto callback = cocos2d::CallFunc::create([this]() {
            this->player->charging = false;
            this->player->attacking = false;
            this->chargeTimer = 0;
        });

        auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);

        ((Node*)this)->stopAllActions();
        ((Node*)this)->runAction(sequence);
    }
}

void Browner::attack() {

    bool attackCondition = false;
    bool chargeCondition = this->player->chargeCondition(); 
    bool dischargeCondition = this->player->dischargeCondition();

    if (this->player->walking && this->canWalkShoot && this->player->attackCondition()) {
        attackCondition = true;
    }
    
    if (!this->player->onGround && this->canJumpShoot && this->player->attackCondition())
    {
        attackCondition = true;
    }

    if (!this->player->walking && this->player->onGround && this->player->attackCondition()) {
        attackCondition = true;
    }
    
    if (attackCondition && !this->player->charging && !this->player->sliding && !this->player->stunned) {
        bool walkLeftCondition = this->player->walkLeftCondition();
        bool walkRightCondition = this->player->walkRightCondition();

        if (walkRightCondition) {
            this->player->sprite->setFlippedX(false);
        }
        else if (walkLeftCondition) {
            this->player->sprite->setFlippedX(true);
        }

        int bulletCount = 0;

        for (int i = 0; i < this->level->entities.size(); ++i) {
            auto entity = this->level->entities.at(i);

            if (entity->getTag() == GameTags::Weapon::WeaponPlayer) {
                bulletCount++;
            }
        }

        if (bulletCount < 3) 
        {
            this->timedShoot();
        }

        this->chargeTimer = 0;
    }

    if (chargeCondition && !this->player->charging) {
        this->player->charging = true;
    }

    if ((dischargeCondition && !this->player->sliding) ||
        (dischargeCondition && this->player->charging && !this->player->sliding) &&
        !this->player->stunned) {
        if (this->chargePower != ChargePower::low) {
            this->timedShoot();
            this->chargePower = ChargePower::low;
        }

        this->player->charging = false;
    }
}
