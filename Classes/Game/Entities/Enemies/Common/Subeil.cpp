#include "Subeil.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/GeometryExtensions.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;


class SubeilResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string SubeilResources::spritePath = "sprites/characters/enemy/general/subeil/subeil";
std::string SubeilResources::armaturePath = "physics/characters/enemy/general/subeil/subeil";

void Subeil::preloadResources() {
    windy::Armature::cache(SubeilResources::armaturePath);
    windy::Sprite::cache(SubeilResources::spritePath);
}


void Subeil::setup() {

    this->power = 2;

    this->maxHealth = 1;
    this->health = this->maxHealth;

    this->attackState = AttackState::Scanning;

    this->walkSpeed = 2;
    this->jumpSpeed = cocos2d::Point(0.5f, 0.75f);

    Enemy::composite(this, SubeilResources::armaturePath, SubeilResources::spritePath, "subeil");


    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",      "subeil_stand",      true,   0.10f),
        windy::AnimationAction("walk",       "subeil_walk",       true,   0.20f),
        windy::AnimationAction("jump",       "subeil_jump",       true,   0.04f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("subeil_stand");
}


std::shared_ptr<cocos2d::Rect> Subeil::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Enemy::buildEntryCollisionRectangle(position, size, SubeilResources::armaturePath, "subeil");
}

void Subeil::attack() {
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
