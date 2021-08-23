#include "FallingSkull.h"

#include "Windy/AnimationAction.h"

#include "Windy/Entities/Player.h"

#include <vector>

using namespace game;

game::Resources& FallingSkull::getResources() {
    static game::Resources resources{ game::ResourceKind::EnemyNight, "falling_skull" };
    return resources;
}

void FallingSkull::setup() {
    maxHealth = 128;
    health = maxHealth;
    power = 6;

    _attackState = AttackState::Idle;

    Logical::composite<FallingSkull>(this);

    std::vector<windy::AnimationAction> actionSet = {
            windy::AnimationAction{"still",  "falling_skull_still",  false, 0.10f},
            windy::AnimationAction{"fall", "falling_skull_fall", true,  0.02f}
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("wall_skull_still");

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;
}

std::shared_ptr<cocos2d::Rect> FallingSkull::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<FallingSkull>(position, size);
}

void FallingSkull::attack(float dt) {
    switch (_attackState)
    {
        case AttackState::Idle:
        {
            float playerDistanceX = cocos2d::Point(this->getPositionX(), 0).getDistance(cocos2d::Point(this->level->player->getPositionX(), 0));

            if (playerDistanceX < 24) {
                _attackState = AttackState::Attacking;
            }

        }
        break;

        case FallingSkull::Attacking:
        {
            this->sprite->runAction("fall");
            this->ignoreGravity = false;
        }
        break;
    }
}
