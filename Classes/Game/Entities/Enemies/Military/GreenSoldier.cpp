#include "GreenSoldier.h"

#include "Windy/AnimationAction.h"
#include "Windy/ObjectManager.h"
#include "Windy/Level.h"

#include "Game/Entities/Weapons/Grenade.h"


using namespace game;

game::Resources& GreenSoldier::getResources() {
    static game::Resources resources{ game::ResourceKind::EnemyMilitary, "green_soldier" };
    return resources;
}

void GreenSoldier::setup() {
    maxHealth = 7;
    health = maxHealth;

    power = 3;

    Logical::composite<GreenSoldier>(this);

    std::vector<windy::AnimationAction> actionSet{
        windy::AnimationAction("stand", "green_soldier_stand", false, 0.10f),
        windy::AnimationAction("attack", "green_soldier_attack",  false, 0.10f)
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("green_soldier_stand");
}

std::shared_ptr<cocos2d::Rect> GreenSoldier::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<GreenSoldier>(position, size);
}

void GreenSoldier::fire() {
    auto bulletPosition = cocos2d::Point(getPositionX(), getPositionY() + 32);
    auto entryCollisionBox = Grenade::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 15));
    bool flipped = sprite->isFlippedX();
    auto entry = Logical::getEntry(entryCollisionBox, [=]() {
        auto bullet = Grenade::create();
        bullet->setPosition(bulletPosition);
        bullet->setup();
        bullet->fire(flipped);

        return bullet;
    }, 512);

    level->objectManager->objectEntries.push_back(entry);
}

void GreenSoldier::attack(float dt) {
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

            auto fireWeapon = cocos2d::CallFunc::create([this]() {
                this->fire();
            });

            auto onEnd = cocos2d::CallFunc::create([this]() {
                this->_attackState = AttackState::Idle;
                this->sprite->runAction("stand");
            });

            auto sequence = cocos2d::Sequence::create(attack, delay, fireWeapon, onEnd, nullptr);
            stopAllActions();
            runAction(sequence);
            _attackState = AttackState::Waiting;

        }
        break;
    }
}
