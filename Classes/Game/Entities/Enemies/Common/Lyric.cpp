#include "Lyric.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/GeometryExtensions.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;


class LyricResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string LyricResources::spritePath = "sprites/characters/enemy/general/lyric/lyric";
std::string LyricResources::armaturePath = "physics/characters/enemy/general/lyric/lyric";

void Lyric::preloadResources() {
    windy::Armature::cache(LyricResources::armaturePath);
    windy::Sprite::cache(LyricResources::spritePath);
}


void Lyric::setup() {

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->power = 3;

    this->maxHealth = 3;
    this->health = this->maxHealth;

    this->moveSpeed = 1.0f;

    this->attackState = AttackState::Attacking;

    Logical::composite<windy::Enemy>(this, LyricResources::armaturePath, LyricResources::spritePath, "subeil");


    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("still",      "lyric_still",      true,   0.10f),
        windy::AnimationAction("fly",        "lyric_fly",        true,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("lyric_fly");
    this->sprite->runAction("fly");
}


std::shared_ptr<cocos2d::Rect> Lyric::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, LyricResources::armaturePath, "subeil");
}


void Lyric::setOrientation() {

}

void Lyric::attack() {
    switch (this->attackState) {

        case AttackState::Attacking: {

            auto displacement = windy::GeometryExtensions::moveTowards(this->getPosition(), this->level->player->getPosition(), (1.0f / 60.0f) * this->moveSpeed);

            this->setPosition(displacement);

        }
        break;


    }
}
