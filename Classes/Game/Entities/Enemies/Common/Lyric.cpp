#include "Lyric.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Windy/AnimationAction.h"
#include "Windy/GeometryExtensions.h"

using namespace game;

game::Resources& Lyric::getResources() {
    static game::Resources resources{game::ResourceKind::EnemyGeneral, "lyric"};
    return resources;
}

void Lyric::setup() {
    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;
    this->power = 3;
    this->maxHealth = 3;
    this->health = this->maxHealth;
    this->moveSpeed = 1.0f;
    this->attackState = AttackState::Attacking;

    Logical::composite<Lyric>(this);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("still",      "lyric_still",      true,   0.10f),
        windy::AnimationAction("fly",        "lyric_fly",        true,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("lyric_fly");
    this->sprite->runAction("fly");
}


std::shared_ptr<cocos2d::Rect> Lyric::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Lyric>(position, size);
}


void Lyric::setOrientation() {

}

void Lyric::attack(float dt) {
    switch (this->attackState) {

        case AttackState::Attacking: {

            auto displacement = windy::GeometryExtensions::moveTowards(this->getPosition(), this->level->player->getPosition(), (1.0f / 60.0f) * this->moveSpeed);

            this->setPosition(displacement);

        }
        break;


    }
}
