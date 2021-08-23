#include "FlashBrain.h"

#include "Windy/AnimationAction.h"

#include "Windy/Level.h"
#include "Windy/Entities/Player.h"

#include <vector>

using namespace game;

game::Resources& FlashBrain::getResources() {
    static game::Resources resources{ game::ResourceKind::EnemyNight, "flash_brain" };
    return resources;
}

void FlashBrain::setup() {
    maxHealth = 3;
    health = maxHealth;
    power = 4;

    _attackState = AttackState::Idle;

    Logical::composite<FlashBrain>(this);

    std::vector<windy::AnimationAction> actionSet = {
            windy::AnimationAction{"idle",  "flash_brain_idle",  true,  0.10f},
            windy::AnimationAction{"walk",  "flash_brain_walk",  true,  0.10f}
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("flash_brain_idle");

}

std::shared_ptr<cocos2d::Rect> FlashBrain::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<FlashBrain>(position, size);
}

void FlashBrain::attack(float dt) {
    switch (_attackState)
    {
        case AttackState::Idle:
        {
            auto delay = cocos2d::DelayTime::create(this->sprite->getActionDuration("idle"));

            auto callback = cocos2d::CallFunc::create([this]() {
                this->sprite->runAction("walk");
                _attackState = AttackState::Attacking;
            });

            auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);

            runAction(sequence);

            _attackState = AttackState::Waiting;

        }
        break;

        case AttackState::Attacking:
        {
            if (this->level->player->getPositionX() < getPositionX()) {
                this->speed.x = -0.75f;
            }
            else {
                this->speed.x = 0.75f;
            }

        }
        break;
    }
}
