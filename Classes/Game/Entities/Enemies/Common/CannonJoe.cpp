#include "CannonJoe.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Game/Entities/Weapons/DirectionalBullet.h"
#include "Windy/AnimationAction.h"
#if _DEBUG
#include "Windy/DebugDrawNode.h"
#endif
#include "Windy/GeometryExtensions.h"
#include "Windy/ObjectManager.h"

using namespace game;

game::Resources& CannonJoe::getResources() {
    static game::Resources resources{game::ResourceKind::EnemyGeneral, "cannon_joe"};
    return resources;
}

void CannonJoe::setup() {

    this->attackTimer = 0;
    this->attackTimeInterval = 1;
    this->bulletPower = 3;
    this->maxHealth = 8;
    this->health = this->maxHealth;
    this->attackState = AttackState::None;

    Logical::composite<CannonJoe>(this);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",      "cannon_joe_stand",      true,   0.10f),
        windy::AnimationAction("flip",       "cannon_joe_flip",       true,   0.24f),
        windy::AnimationAction("shoot",      "cannon_joe_shoot",      true,   0.04f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("cannon_joe_stand");
}

std::shared_ptr<cocos2d::Rect> CannonJoe::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<CannonJoe>(position, size);
}

void CannonJoe::setOrientation() {
    
}

void CannonJoe::attack(float dt) {
    switch (this->attackState) {

        case AttackState::None: {

            auto& playerCollisionBox = *this->level->player->collisionBox;
            auto projectedCollisionBoxLeft = *this->collisionBox;

            projectedCollisionBoxLeft.origin.x -= 172;
            projectedCollisionBoxLeft.origin.y -= 112;
            projectedCollisionBoxLeft.size.width = 172;
            projectedCollisionBoxLeft.size.height = 224;

            //this->level->debugDrawNode->drawCollisionBox(projectedCollisionBoxLeft);

            if (windy::GeometryExtensions::rectIntersectsRect(playerCollisionBox, projectedCollisionBoxLeft)) {
                this->attackState = AttackState::Before;
                this->attackTimer = this->attackTimeInterval;
            }

        }
        break;

        case AttackState::Before: {
            if (this->attackTimer <= 0) {
                this->attackTimer = 0;

                auto& playerCollisionBox = *this->level->player->collisionBox;
                auto projectedCollisionBoxLeft = *this->collisionBox;

                projectedCollisionBoxLeft.origin.x -= 172;
                projectedCollisionBoxLeft.origin.y -= 112;
                projectedCollisionBoxLeft.size.width = 172;
                projectedCollisionBoxLeft.size.height = 224;

                //this->level->debugDrawNode->drawCollisionBox(projectedCollisionBoxLeft);


                if (windy::GeometryExtensions::rectIntersectsRect(playerCollisionBox, projectedCollisionBoxLeft)) {
                    auto playerPosition = this->level->player->getPosition();

                    float playerDistanceX = cocos2d::Point(this->getPositionX(), 0).getDistance(cocos2d::Point(this->level->player->getPositionX(), 0));
                    //float playerDistanceY = cocos2d::Point(0, this->getPositionY()).getDistance(cocos2d::Point(0, this->level->player->getPositionY()));

                    auto bulletPosition = cocos2d::Point(this->getPositionX() + (22 * this->getSpriteNormal() * -1), this->getPositionY());

                    auto entryCollisionBox = DirectionalBullet::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 16));
                    auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                        auto bullet = DirectionalBullet::create();
                        bullet->setPosition(bulletPosition);
                        bullet->setup();

                        if (playerDistanceX < 48){
                            bullet->fire(this->bulletPower, this->getSpriteNormal() * -1, windy::GameTags::WeaponEnemy);
                        }
                        else {
                            bullet->fire(this->bulletPower, playerPosition, windy::GameTags::WeaponEnemy);
                        }


                        return bullet;
                        });



                    this->level->objectManager->objectEntries.push_back(entry);

                    this->attackTimer = this->attackTimeInterval;
                    this->attackState = AttackState::Cooldown;
                }

            }
            else {
                this->attackTimer -= dt;
            }


        }
        break;

        case AttackState::Cooldown:
        {
            if (this->attackTimer <= 0) {
                this->attackTimer = 0;
                this->attackState = AttackState::None;
            }
            else {
                this->attackTimer -= dt;
            }


        }
        break;

    }
}
