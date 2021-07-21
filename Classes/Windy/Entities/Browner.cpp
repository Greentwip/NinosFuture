#include "Browner.h"

#include "./../Level.h"
#include "./../Sprite.h"
#include "./../Input.h"

#include "./../GameTags.h"

#include "./../AudioManager.h"

#include "Player.h"

#include "VioletBullet.h"


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
    this->canCharge = true;
}

void Browner::initVariables() 
{
    this->chargeTimer = 0;
    this->attackTimer = 0;

    this->energy = 28;

    this->chargePower = "low";

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

void Browner::activate() {
    this->player->sprite->setVisible(true);
}

void Browner::deactivate() {
    this->stopActions();
    this->player->sprite->setVisible(false);
}

void Browner::runAction(const std::string& action) {
    this->player->sprite->setAnimation(action + "/" + this->baseName + "_" + action);
    this->player->sprite->runAction(action, this->baseName);
}

float Browner::getActionDuration(const std::string& action) {
    return this->player->sprite->getActionDuration(action);
}

int Browner::getCurrentAnimationNumberOfFrames() {
    return this->player->sprite->getCurrentAnimationNumberOfFrames();
}

void Browner::stopActions() {
    this->player->sprite->stopActions();
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
            this->chargePower = "mid";
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

            this->chargePower = "high";

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

    bool attackCondition = Input::keyPressed(InputKey::B);
    bool chargeCondition = Input::keyDown(InputKey::B);
    bool dischargeCondition = !Input::keyDown(InputKey::B);
    
    if (attackCondition && !this->player->charging && !this->player->sliding && !this->player->stunned) {
        bool walkLeftCondition = Input::keyDown(InputKey::Right) && !Input::keyDown(InputKey::Left);
        bool walkRightCondition = Input::keyDown(InputKey::Right) && !Input::keyDown(InputKey::Left);

        if (walkLeftCondition) {
            this->player->sprite->setFlippedX(false);
        }
        else if (walkRightCondition) {
            this->player->sprite->setFlippedX(true);
        }

        int bulletCount = 0;

        for (int i = 0; i < this->level->entities.size(); ++i) {
            auto entity = this->level->entities.at(i);

            if (entity->getTag() == GameTags::Weapon::WeaponPlayer) {
                bulletCount++;
            }
        }

        //if (bulletCount < 3) 
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
        if (this->chargePower.compare("low") != 0) {
            this->timedShoot();
            this->chargePower = "low";
        }

        this->player->charging = false;
    }
}

void Browner::fire() {

    int bulletOffset = 12;
    int bulletPower = 1;

    if (this->chargePower.compare("high") == 0) {
        bulletOffset = 26;
        bulletPower = 3;
        AudioManager::playSfx(Sounds::BusterHigh);
    }
    else if (this->chargePower.compare("mid") == 0) {
        bulletPower = 2;
        AudioManager::playSfx(Sounds::BusterMid);
    }
    else {
        AudioManager::playSfx(Sounds::BusterLow);
    }

    auto bulletPosition = cocos2d::Point(this->player->getPositionX() + (bulletOffset * this->getSpriteNormal()),
                                         this->player->getPositionY() - 4);

    auto bullet = VioletBullet::create();
    bullet->setPosition(bulletPosition);
    bullet->setup(this->chargePower);

    bullet->fire(bulletPower, this->getSpriteNormal(), GameTags::WeaponPlayer);


    this->level->entities.pushBack(bullet);
    this->level->addChild(bullet, 2048);

    //self:getParent():getParent().bullets_[bullet] = bullet

}