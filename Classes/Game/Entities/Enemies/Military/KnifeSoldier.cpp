#include "KnifeSoldier.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Game/Entities/Weapons/Knife.h"
#include "Windy/AnimationAction.h"
#if _DEBUG
#include "Windy/DebugDrawNode.h"
#endif
#include "Windy/GeometryExtensions.h"
#include "Windy/ObjectManager.h"

using namespace game;

game::Resources& KnifeSoldier::getResources() {
    static game::Resources resources{ game::ResourceKind::EnemyMilitary, "claw_soldier" };
    return resources;
}

void KnifeSoldier::setup() {
    this->bulletPower = 5;
    this->maxHealth = 8;
    this->health = this->maxHealth;
    this->_attackState = AttackState::Idle;

    Logical::composite<KnifeSoldier>(this);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",      "claw_soldier_stand",      false,   0.10f),
        windy::AnimationAction("attack",     "claw_soldier_attack",     false,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("claw_soldier_stand");
}

std::shared_ptr<cocos2d::Rect> KnifeSoldier::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<KnifeSoldier>(position, size);
}


void KnifeSoldier::fire() {
    auto bulletPosition = cocos2d::Point(getPositionX() + static_cast<float>(20 * getSpriteNormal() * -1), getPositionY());
    auto entryCollisionBox = Knife::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 15));
    bool flipped = sprite->isFlippedX();
    auto entry = Logical::getEntry(entryCollisionBox, [=]() {
        float playerDistanceX = cocos2d::Point(this->getPositionX(), 0).getDistance(cocos2d::Point(this->level->player->getPositionX(), 0));
        float playerDistanceY = cocos2d::Point(0, this->getPositionY()).getDistance(cocos2d::Point(0, this->level->player->getPositionY()));

        auto playerPosition = this->level->player->getPosition();

        auto bullet = Knife::create();
        bullet->setPosition(bulletPosition);
        bullet->setup();

        if (playerDistanceX < 32) {
            bullet->fire(this->bulletPower, this->getSpriteNormal() * -1, windy::GameTags::WeaponEnemy);
        }
        else {
            bullet->fire(this->bulletPower, playerPosition, windy::GameTags::WeaponEnemy);
        }

        return bullet;
        });

    level->objectManager->objectEntries.push_back(entry);
}


void KnifeSoldier::attack(float dt) {
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

            auto freezeDelay = cocos2d::DelayTime::create(0.25f);

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
