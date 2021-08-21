#include "RollRunner.h"

#include "Windy/AnimationAction.h"

using namespace game;

game::Resources& RollRunner::getResources() {
    static game::Resources resources{game::ResourceKind::EnemyMilitary, "roll_runner"};
    return resources;
}

void RollRunner::setOrientation() {

}

void RollRunner::setup() {
    maxHealth = 5;
    power = 3;

    orientationSet = false;
    walkSpeed = 1.0f;
    orientation = Orientation::Left;

    Logical::composite<RollRunner>(this);

    std::vector<windy::AnimationAction> actionSet {
        windy::AnimationAction("walk", "roll_runner_walk", true, 0.10f)
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("roll_runner_walk");
}

std::shared_ptr<cocos2d::Rect> RollRunner::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<RollRunner>(position, size);
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

        speed.x = walkSpeed * (float)orientation;
    }
}
