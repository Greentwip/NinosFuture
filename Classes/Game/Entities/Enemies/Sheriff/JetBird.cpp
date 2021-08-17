#include "JetBird.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Windy/AnimationAction.h"

using namespace game;

game::Resources& JetBird::getResources() {
    static game::Resources resources{game::ResourceKind::EnemySheriff, "jetbird"};
    return resources;
}

void JetBird::setup() {

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;
    this->power = 4;
    this->maxHealth = 3;
    this->health = this->maxHealth;
    this->attackState = AttackState::Scanning;
    this->flySpeed = 1;

    Logical::composite<JetBird>(this);

    auto action = windy::AnimationAction("fly", "jetbird_fly", true, 0.10f);

    this->sprite->appendAction(action, false);
    this->sprite->setAnimation("jetbird_fly");
}


std::shared_ptr<cocos2d::Rect> JetBird::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<JetBird>(position, size);
}

void JetBird::setOrientation() {

}

void JetBird::attack(float dt) {
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
