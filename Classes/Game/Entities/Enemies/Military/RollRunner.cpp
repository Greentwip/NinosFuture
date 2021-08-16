#include "RollRunner.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

using namespace game;

class RollRunnerResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string RollRunnerResources::spritePath = "sprites/characters/enemy/military/roll_runner/roll_runner";
std::string RollRunnerResources::armaturePath = "physics/characters/enemy/military/roll_runner/roll_runner";

void RollRunner::preloadResources() {
    windy::Armature::cache(RollRunnerResources::armaturePath);
    windy::Sprite::cache(RollRunnerResources::spritePath);
}

void RollRunner::setup() {
    maxHealth = 5;
    power = 3;

    Logical::composite<windy::Enemy>(this, RollRunnerResources::armaturePath, RollRunnerResources::spritePath, "roll_runner");

    std::vector<windy::AnimationAction> actionSet {
        windy::AnimationAction("walk", "roll_runner_walk", true, 0.10f)
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("roll_runner_walk");
}

std::shared_ptr<cocos2d::Rect> RollRunner::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, RollRunnerResources::armaturePath, "roll_runner");
}

void RollRunner::flip(Orientation _orientation) {
    bool flipped = (_orientation == Orientation::Right);
    sprite->setFlippedX(flipped);
    orientation = _orientation;
}

void RollRunner::attack(float dt) {
    if (contacts[windy::CollisionContact::Down]) {
        if (contacts[windy::CollisionContact::Right]) {
            flip(Orientation::Left);
        } else if (contacts[windy::CollisionContact::Left]) {
            flip(Orientation::Right);
        }

        speed.x += walkSpeed * (float)orientation;
    }
}
