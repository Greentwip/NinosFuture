#include "Rip.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Windy/AnimationAction.h"
#include "Windy/AudioManager.h"

using namespace game;

game::Resources& Rip::getResources() {
    static game::Resources resources{game::ResourceKind::EnemyNight, "rip_jumper"};
    return resources;
}

void Rip::setup() {
    this->power = 6;
    this->maxHealth = 6;
    this->health = this->maxHealth; 
    this->attackTimer = 0;
    this->attackTimeInterval = 1;
    this->attackState = AttackState::None;
    this->jumpSpeed = cocos2d::Point(1, 5);

    Logical::composite<Rip>(this);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",      "rip_jumper_stand",      true,   0.10f),
        windy::AnimationAction("jump",       "rip_jumper_jump",       false,   0.05f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("rip_jumper_stand");
    this->sprite->runAction("stand");
}

std::shared_ptr<cocos2d::Rect> Rip::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Rip>(position, size);
}

void Rip::attack(float dt) {
    switch (this->attackState) {

        case AttackState::None: {
            float playerDistance = this->getPosition().getDistance(this->level->player->getPosition());
            

            if (playerDistance < 100) {
                this->attackState = AttackState::Jumping;
                this->attackTimer = this->attackTimeInterval;
            }

        }
        break;

        case AttackState::Jumping: {
            if (this->attackTimer <= 0) {
                this->attackTimer = 0;

                this->speed.x = this->jumpSpeed.x * this->getSpriteNormal() * -1;
                this->speed.y = this->jumpSpeed.y;

                //windy::AudioManager::playSfx(windy::Sounds::Roar);

                this->sprite->runAction("jump");

                this->attackState = AttackState::Attacking;
            }
            else {
                this->attackTimer -= dt;
            }


        }
        break;

        case AttackState::Attacking: {
            if (this->contacts[windy::CollisionContact::Down]) {
                this->speed.x = 0;
                this->sprite->runAction("stand");
                this->attackState = AttackState::None;
            }
        }
        break;

    }
}
