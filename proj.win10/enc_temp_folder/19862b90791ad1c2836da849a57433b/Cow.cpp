#include "Cow.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/GameTags.h"

#include "Windy/ObjectManager.h"

#include "Game/Entities/Weapons/DirectionalBullet.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;


class CowResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string CowResources::spritePath = "sprites/characters/enemy/sheriff/cow/cow";
std::string CowResources::armaturePath = "physics/characters/enemy/sheriff/cow/cow";

void Cow::preloadResources() {
    windy::Armature::cache(CowResources::armaturePath);
    windy::Sprite::cache(CowResources::spritePath);
}


void Cow::setup() {

    this->maxHealth = 28;
    this->health = this->maxHealth;

    this->bulletPower = 5;
    this->attackState = AttackState::Scanning;

    Enemy::composite(this, CowResources::armaturePath, CowResources::spritePath, "cow");

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",       "cow_stand",      false,   0.10f),
        windy::AnimationAction("morph",       "cow_flip",       false,   0.12f),
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
    return Enemy::buildEntryCollisionRectangle(position, size, CowResources::armaturePath, "cow");
}

void Cow::attack() {
    switch (this->attackState) {

        case AttackState::Scanning: {
            float playerDistance = this->getPosition().getDistance(this->level->player->getPosition());

            if (playerDistance < 64) {
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
                morphTimer -= 1.0f / 60.0f;
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

                    auto bulletPosition = cocos2d::Point(this->getPositionX() + (22 * this->getSpriteNormal() * -1),
                        this->getPositionY() - 24);

                    auto entryCollisionBox = DirectionalBullet::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 16));
                    auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                        auto bullet = DirectionalBullet::create();
                        bullet->setPosition(bulletPosition);
                        bullet->setup();

                        if (playerDistanceX < 30 && playerDistanceY < 16) {
                            bullet->fire(this->bulletPower, this->getSpriteNormal(), windy::GameTags::WeaponEnemy);
                        }
                        else {
                            bullet->fire(this->bulletPower, playerPosition, windy::GameTags::WeaponEnemy);
                        }


                        return bullet;
                        });



                    this->level->objectManager->objectEntries.push_back(entry);

                    this->sprite->reverseAction();

                    this->attackTimer = this->attackTimeInterval * 2;
                    this->attackState = AttackState::CoolDown;
                }

            }
            else {
                this->attackTimer -= 1.0f / 60.0f;
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
                this->attackTimer -= 1.0f / 60.0f;
            }


        }
        break;

    }
}
