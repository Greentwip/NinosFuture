#include "WallSkull.h"

#include "Windy/AnimationAction.h"

#include <vector>

using namespace game;

game::Resources& WallSkull::getResources() {
    static game::Resources resources{ game::ResourceKind::EnemyNight, "wall_skull" };
    return resources;
}

void WallSkull::setup() {
    maxHealth = 128;
    health = maxHealth;
    power = 6;

    _attackState = AttackState::Idle;

    Logical::composite<WallSkull>(this);

    std::vector<windy::AnimationAction> actionSet = {
            windy::AnimationAction{"still",  "wall_skull_still",  false, 0.10f},
            windy::AnimationAction{"attack", "wall_skull_attack", true,  0.10f}
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("wall_skull_still");

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;
}

std::shared_ptr<cocos2d::Rect> WallSkull::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<WallSkull>(position, size);
}

void WallSkull::attack(float dt) {
    switch (_attackState)
    {
        case AttackState::Idle:
        {
            auto delay = cocos2d::DelayTime::create(1);

            auto callback = cocos2d::CallFunc::create([this]() {
                _attackState = AttackState::Attacking;
            });

            auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);

            runAction(sequence);

            _attackState = AttackState::Waiting;

        }
        break;

        case AttackState::Attacking:
        {
            auto delay = cocos2d::DelayTime::create(this->sprite->getActionDuration("attack"));
            
            auto attack = cocos2d::CallFunc::create([this]() {
                this->sprite->runAction("attack");
            });

            auto onEnd = cocos2d::CallFunc::create([this]() {
                this->_attackState = AttackState::Idle;
                this->sprite->runAction("still");
            });

            auto sequence = cocos2d::Sequence::create(attack, delay, onEnd, nullptr);
            stopAllActions();
            runAction(sequence);
            _attackState = AttackState::Waiting;

        }
        break;
    }
}
