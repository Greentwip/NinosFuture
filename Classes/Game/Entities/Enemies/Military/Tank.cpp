#include "Tank.h"

#include "Game/Entities/Player/GamePlayer.h"
#include "Game/Entities/Weapons/TankBullet.h"
#include "Windy/AnimationAction.h"
#include "Windy/ObjectManager.h"

#include <vector>

using namespace game;

game::Resources& Tank::getResources() {
    static game::Resources resources{game::ResourceKind::EnemyMilitary, "tank"};
    return resources;
}

void Tank::setup() {
    maxHealth = 24;
    health = maxHealth;
    power = 6;

    _walkSpeed = 0.5f;
    _attackState = AttackState::Walking;

    Logical::composite<Tank>(this);

    std::vector<windy::AnimationAction> actionSet = {
            windy::AnimationAction{"stand", "tank_stand", true, 0.10f},
            windy::AnimationAction{"walk", "tank_walk", true, 0.10f}
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("tank_walk");
}

std::shared_ptr<cocos2d::Rect> Tank::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Tank>(position, size);
}

void Tank::fire() {
    if (_attackState == AttackState::Attacking) {
        auto bulletPosition = cocos2d::Point(getPositionX() + (float)(18 * -getSpriteNormal()), getPositionY() + 18);
        auto entryCollisionBox = TankBullet::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 15));
        bool flipped = sprite->isFlippedX();
        auto entry = Logical::getEntry(entryCollisionBox, [=]() {
            auto* bullet = TankBullet::create();
            bullet->setPosition(bulletPosition);
            bullet->setup();
            bullet->fire(flipped);

            return bullet;
        });

        level->objectManager->objectEntries.push_back(entry);
    }
}

void Tank::attack(float dt) {
    if (_attackState == AttackState::Walking) {
        float playerDistance = getPosition().getDistance(level->player->getPosition());
        bool inRange = (playerDistance < 72.0f);

        if (!inRange && level->player->getPositionX() >= getPositionX()) {
            speed.x = _walkSpeed;
            sprite->setFlippedX(true);
        } else if (!inRange) {
            speed.x = -_walkSpeed;
            sprite->setFlippedX(false);
        }

        if (inRange || contacts[windy::CollisionContact::Right] || contacts[windy::CollisionContact::Left]) {
            speed.x = 0;
            sprite->stopActions();
            _attackState = AttackState::Standing;
        }

        if (sprite->getCurrentAction() != sprite->getAction("walk")) {
            sprite->runAction("walk");
        }
    } else if (_attackState == AttackState::Standing) {
        auto delay = cocos2d::DelayTime::create(2.0f);

        auto fireWeapon = cocos2d::CallFunc::create([this]() {
            this->fire();
        });

        auto onEnd = cocos2d::CallFunc::create([this]() {
            this->_attackState = AttackState::Walking;
            this->sprite->runAction("stand");
        });

        auto sequence = cocos2d::Sequence::create(delay, fireWeapon, delay, onEnd, nullptr);
        stopAllActions();
        runAction(sequence);
        _attackState = AttackState::Attacking;
    }
}
