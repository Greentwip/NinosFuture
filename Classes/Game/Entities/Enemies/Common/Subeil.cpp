#include "Subeil.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Windy/AnimationAction.h"
#include "Windy/GeometryExtensions.h"

using namespace game;

windy::Resources& Subeil::getResources() {
    static windy::Resources resources{windy::ResourceKind::EnemyGeneral, "subeil"};
    return resources;
}

void Subeil::setup() {
    this->power = 2;
    this->maxHealth = 1;
    this->health = this->maxHealth;
    this->attackState = AttackState::Scanning;
    this->walkSpeed = 2;
    this->jumpSpeed = cocos2d::Point(0.5f, 0.75f);

    Logical::composite<Subeil>(this);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",      "subeil_stand",      true,   0.10f),
        windy::AnimationAction("walk",       "subeil_walk",       true,   0.20f),
        windy::AnimationAction("jump",       "subeil_jump",       true,   0.04f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("subeil_stand");
}


std::shared_ptr<cocos2d::Rect> Subeil::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Subeil>(position, size);
}

void Subeil::attack(float dt) {
    switch (this->attackState) {

        case AttackState::Scanning: {

            auto& playerCollisionBox = *this->level->player->collisionBox;
            auto projectedCollisionBoxRight = *this->collisionBox;
            auto projectedCollisionBoxLeft = *this->collisionBox;

            projectedCollisionBoxRight.size.width = 128;

            projectedCollisionBoxLeft.origin.x -= 128;
            projectedCollisionBoxLeft.size.width = 128;

            if (windy::GeometryExtensions::rectIntersectsRect(playerCollisionBox, projectedCollisionBoxRight) ||
                windy::GeometryExtensions::rectIntersectsRect(playerCollisionBox, projectedCollisionBoxLeft)){
                this->sprite->runAction("walk");

                this->speed.x = static_cast<float>(this->walkSpeed * this->getSpriteNormal());

            }            
            else {
                this->sprite->runAction("stand");
                this->speed.x = 0;
            }
            
            float playerDistance = this->getPosition().getDistance(this->level->player->getPosition());

            if (playerDistance < 32) {
                this->attackState = AttackState::Jumping;
            }

        }
        break;

        case AttackState::Jumping: {
            this->speed.x = this->jumpSpeed.x * this->getSpriteNormal();
            this->speed.y = this->jumpSpeed.y;

            this->sprite->stopActions();
            this->sprite->runAction("jump");

            this->attackState = AttackState::Attacking;


        }
        break;

        case AttackState::Attacking: {
            if (this->contacts[windy::CollisionContact::Down]) {
                this->speed.x = 0;
                this->sprite->stopActions();
                this->sprite->runAction("stand");
                this->attackState = AttackState::Scanning;
            }
        }
        break;

    }
}
