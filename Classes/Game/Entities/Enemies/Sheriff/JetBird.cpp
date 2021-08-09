#include "JetBird.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;


class JetBirdResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string JetBirdResources::spritePath = "sprites/characters/enemy/sheriff/jetbird/jetbird";
std::string JetBirdResources::armaturePath = "physics/characters/enemy/sheriff/jetbird/jetbird";

void JetBird::preloadResources() {
    windy::Armature::cache(JetBirdResources::armaturePath);
    windy::Sprite::cache(JetBirdResources::spritePath);
}


void JetBird::setup() {

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->power = 4;

    this->maxHealth = 4;
    this->health = this->maxHealth;

    this->attackState = AttackState::Scanning;

    this->flySpeed = 1;

    Logical::composite<windy::Enemy>(this, JetBirdResources::armaturePath, JetBirdResources::spritePath, "jetbird");

    auto action = windy::AnimationAction("fly", "jetbird_fly", true, 0.10f);

    this->sprite->appendAction(action, false);
    this->sprite->setAnimation("jetbird_fly");
}


std::shared_ptr<cocos2d::Rect> JetBird::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, JetBirdResources::armaturePath, "jetbird");
}


void JetBird::setOrientation() {

}

void JetBird::attack() {
    switch (this->attackState) {
        case AttackState::Scanning:{

            if (this->level->player->getPositionX() > this->getPositionX()) {
                this->sprite->setFlippedX(true);
            }
            
            this->attackState = AttackState::Flying;
        }
        break;

        case AttackState::Flying: {
            this->speed.x = this->flySpeed * this->getSpriteNormal() * -1;
        }
        break;
    }
}
