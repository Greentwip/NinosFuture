#include "Sumatran.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/AudioManager.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;


class SumatranResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string SumatranResources::spritePath = "sprites/characters/enemy/general/sumatran/sumatran";
std::string SumatranResources::armaturePath = "physics/characters/enemy/general/sumatran/sumatran";

void Sumatran::preloadResources() {
    windy::Armature::cache(SumatranResources::armaturePath);
    windy::Sprite::cache(SumatranResources::spritePath);
}


void Sumatran::setup() {

    this->power = 6;

    this->maxHealth = 12;
    this->health = this->maxHealth; 

    this->attackTimer = 0;
    this->attackTimeInterval = 1;
    this->attackState = AttackState::None;

    this->jumpSpeed = cocos2d::Point(1, 5);

    Logical::composite<windy::Enemy>(this, SumatranResources::armaturePath, SumatranResources::spritePath, "sumatran");

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",      "sumatran_stand",      true,   0.10f),
        windy::AnimationAction("jump",       "sumatran_jump",       false,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("sumatran_stand");
    this->sprite->runAction("stand");
}


std::shared_ptr<cocos2d::Rect> Sumatran::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, SumatranResources::armaturePath, "sumatran");
}


void Sumatran::attack(float dt) {
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

                windy::AudioManager::playSfx(windy::Sounds::Roar);

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
