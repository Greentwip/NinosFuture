#include "Cow.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Game/Entities/Weapons/DirectionalBullet.h"
#include "Windy/AnimationAction.h"
#include "Windy/GameTags.h"
#include "Windy/ObjectManager.h"

using namespace game;

windy::Resources& Cow::getResources() {
    static windy::Resources resources {windy::ResourceKind::EnemySheriff, "cow"};
    return resources;
}

void Cow::setup() {

    this->maxHealth = 40;
    this->health = this->maxHealth;

    this->bulletPower = 5;
    this->attackState = AttackState::Scanning;

    Logical::composite<Cow>(this);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",       "cow_stand",      false,   0.10f),
        windy::AnimationAction("morph",       "cow_morph",       false,   0.12f),
        windy::AnimationAction("attack",      "cow_attack",     false,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("cow_stand");

    this->attackTimer = 0;
    this->morphTimer = 0;

    this->attackTimeInterval = this->sprite->getActionDuration("attack");
    this->morphTimeInterval = this->sprite->getActionDuration("morph");

}

std::shared_ptr<cocos2d::Rect> Cow::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Cow>(position, size);
}

void Cow::attack(float dt) {
    switch (this->attackState) {

        case AttackState::Scanning: {
            float playerDistance = this->getPosition().getDistance(this->level->player->getPosition());

            if (playerDistance < 128) {
                this->sprite->runAction("morph");
                this->attackState = AttackState::Morphing;
                this->morphTimer = this->morphTimeInterval;
            }

        }
        break;

        case AttackState::Morphing: {
            if (this->morphTimer <= 0) {
                this->attackTimer = this->attackTimeInterval;
                this->sprite->runAction("attack");
                this->attackState = AttackState::Attacking;
            }
            else {
                morphTimer -= dt;
            }
        }
        break;

        case AttackState::Attacking: {
            if (this->attackTimer <= 0) {
                this->attackTimer = 0;

                float playerDistanceX = cocos2d::Point(this->getPositionX(), 0).getDistance(cocos2d::Point(this->level->player->getPositionX(), 0));
                float playerDistanceY = cocos2d::Point(0, this->getPositionY()).getDistance(cocos2d::Point(0, this->level->player->getPositionY()));


                if (playerDistanceX > 48 || playerDistanceY <= 24) {
                    auto playerPosition = this->level->player->getPosition();

                    auto bulletPosition = 
                        cocos2d::Point(
                            this->getPositionX() + (30 * this->getSpriteNormal() * -1),
                            this->getPositionY() - 2);

                    auto entryCollisionBox = DirectionalBullet::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 16));
                    auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                        auto bullet = DirectionalBullet::create();
                        bullet->setPosition(bulletPosition);
                        bullet->setup();

                        if (playerDistanceX < 32) {
                            bullet->fire(this->bulletPower, this->getSpriteNormal() * -1, windy::GameTags::WeaponEnemy);
                        }
                        else {
                            bullet->fire(this->bulletPower, playerPosition, windy::GameTags::WeaponEnemy);
                        }


                        return bullet;
                        });



                    this->level->objectManager->objectEntries.push_back(entry);

                    this->sprite->reverseAction();

                    this->attackTimer = this->attackTimeInterval * 6;
                    this->attackState = AttackState::CoolDown;
                }

            }
            else {
                this->attackTimer -= dt;
            }


        }
        break;

        case AttackState::CoolDown:
        {
            if (this->attackTimer <= 0) {
                this->attackTimer = this->attackTimeInterval;
                this->sprite->reverseAction();
                this->attackState = AttackState::Attacking;
            }
            else {
                this->attackTimer -= dt;
            }


        }
        break;

    }
}
