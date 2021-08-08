#include "Taban.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/AudioManager.h"

#include "Windy/GameTags.h"

#include "Windy/GeometryExtensions.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;


class TabanResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string TabanResources::spritePath = "sprites/characters/enemy/general/taban/taban";
std::string TabanResources::armaturePath = "physics/characters/enemy/general/taban/taban";

void Taban::preloadResources() {
    windy::Armature::cache(TabanResources::armaturePath);
    windy::Sprite::cache(TabanResources::spritePath);
}


void Taban::setup() {

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->power = 3;

    this->maxHealth = 3;
    this->health = this->maxHealth;

    this->attackState = AttackState::Scanning;

    this->moveSpeed = 16;

    Enemy::composite(this, TabanResources::armaturePath, TabanResources::spritePath, "taban");

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("still",      "taban_still",      true,   0.10f),
        windy::AnimationAction("wake",       "taban_wake",       true,   0.50f),
        windy::AnimationAction("fly",        "taban_fly",        true,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);
    this->sprite->setAnimation("taban_still");
    this->sprite->runAction("still");


    this->wakeUpDelay = this->sprite->getActionDuration("wake");
    this->wakeUpTimer = 0;
}


std::shared_ptr<cocos2d::Rect> Taban::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Enemy::buildEntryCollisionRectangle(position, size, TabanResources::armaturePath, "taban");
}


void Taban::setOrientation() {

    if (this->attackState == AttackState::Attacking) {
        Enemy::setOrientation();
    }
    
}

void Taban::attack() {
    switch (this->attackState) {

        case AttackState::Scanning: {

            float playerDistanceX = cocos2d::Point(this->getPosition().x, 0).getDistance(cocos2d::Point(this->level->player->getPositionX(), 0));

            if (playerDistanceX < 72 && this->level->player->getPositionX() < this->getPositionX()) {
                this->wakeUpTimer = this->wakeUpDelay;
                this->sprite->runAction("wake");
                this->attackState = AttackState::WakingUp;
            }


        }
        break;

        case AttackState::WakingUp: {

            if (this->wakeUpTimer <= 0) {
                windy::AudioManager::playSfx(windy::Sounds::Taban);
                this->sprite->runAction("fly");
                this->attackState = AttackState::Attacking;
            }
            else {
                this->wakeUpTimer -= 1.0f / 60.0f;
            }
            


        }
        break;

        case AttackState::Attacking: {
            auto displacement = windy::GeometryExtensions::moveTowards(this->getPosition(), this->level->player->getPosition(), (1.0f / 60.0f) * this->moveSpeed);

            this->setPosition(displacement);
        }
        break;

    }
}
