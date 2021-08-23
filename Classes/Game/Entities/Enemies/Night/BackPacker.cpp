#include "BackPacker.h"

#include "Windy/AnimationAction.h"
#include "Windy/ObjectManager.h"
#include "Windy/Level.h"

#include "Game/Entities/Weapons/BackPackBullet.h"

using namespace game;

game::Resources& BackPacker::getResources() {
    static game::Resources resources{ game::ResourceKind::EnemyNight, "backpacker" };
    return resources;
}

void BackPacker::setup() {
    maxHealth = 12;
    health = maxHealth;

    power = 3;

    Logical::composite<BackPacker>(this);

    std::vector<windy::AnimationAction> actionSet{
        windy::AnimationAction("stand", "backpacker_stand", false, 0.10f),
        windy::AnimationAction("attack", "backpacker_attack",  false, 0.10f)
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("backpacker_attack");
}

std::shared_ptr<cocos2d::Rect> BackPacker::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<BackPacker>(position, size);
}

void BackPacker::fire() {
    auto bulletPosition = cocos2d::Point(getPositionX() + static_cast<float>(-18 * getSpriteNormal() * -1), getPositionY() + 56);
    auto entryCollisionBox = BackPackBullet::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 15));
    bool flipped = sprite->isFlippedX();
    auto entry = Logical::getEntry(entryCollisionBox, [=]() {
        auto bullet = BackPackBullet::create();
        bullet->setPosition(bulletPosition);
        bullet->setup();
        bullet->fire(flipped);

        return bullet;
    });

    level->objectManager->objectEntries.push_back(entry);
}

void BackPacker::attack(float dt) {
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

            auto freezeDelay = cocos2d::DelayTime::create(0.5f);

            auto onEnd = cocos2d::CallFunc::create([this]() {
                this->_attackState = AttackState::Idle;
                this->sprite->runAction("stand");
            });

            auto sequence = cocos2d::Sequence::create(attack, delay, fireWeapon, freezeDelay, onEnd, nullptr);
            stopAllActions();
            runAction(sequence);
            _attackState = AttackState::Waiting;

        }
        break;
    }
}
