#include "RedSoldier.h"

#include "Windy/AnimationAction.h"
#include "Windy/ObjectManager.h"
#include "Windy/Level.h"
#include "Windy/PhysicsWorld.h"
#include "Windy/Entities/Player.h"

#include "Game/Entities/Weapons/DirectionalBullet.h"


using namespace game;

game::Resources& RedSoldier::getResources() {
    static game::Resources resources{ game::ResourceKind::EnemyMilitary, "red_soldier" };
    return resources;
}

void RedSoldier::setup() {
    maxHealth = 7;
    health = maxHealth;

    power = 3;

    bulletPower = 4;

    Logical::composite<RedSoldier>(this);

    std::vector<windy::AnimationAction> actionSet{
        windy::AnimationAction("stand",             "red_soldier_stand",   false, 0.10f),
        windy::AnimationAction("attack",            "red_soldier_attack",  false, 0.10f),
        windy::AnimationAction("fall",              "red_soldier_fall",    false, 0.10f),
        windy::AnimationAction("morph_a",           "red_soldier_morph_a", false, 0.15f),
        windy::AnimationAction("morph_a_reversed",  "red_soldier_morph_a", false, 0.15f),
        windy::AnimationAction("morph_b",           "red_soldier_morph_b", false, 0.15f)
    };

    sprite->appendActionSet(actionSet, false);
    
    sprite->runAction("morph_a_reversed");
    sprite->reverseAction();

    sprite->stopActions();

    sprite->setAnimation("red_soldier_fall");

    this->ignoreGravity = true;

    _attackState = AttackState::Falling;

}

std::shared_ptr<cocos2d::Rect> RedSoldier::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<RedSoldier>(position, size);
}

void RedSoldier::fire() {
    auto bulletPosition = cocos2d::Point(getPositionX() + static_cast<float>(8 * getSpriteNormal() * -1), getPositionY() - 4);
    auto entryCollisionBox = DirectionalBullet::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 15));
    bool flipped = sprite->isFlippedX();
    auto entry = Logical::getEntry(entryCollisionBox, [=]() {
        float playerDistanceX = cocos2d::Point(this->getPositionX(), 0).getDistance(cocos2d::Point(this->level->player->getPositionX(), 0));
        float playerDistanceY = cocos2d::Point(0, this->getPositionY()).getDistance(cocos2d::Point(0, this->level->player->getPositionY()));

        auto playerPosition = this->level->player->getPosition();

        auto bullet = DirectionalBullet::create();
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

void RedSoldier::attack(float dt) {
    switch (_attackState)
    {
        case AttackState::Falling:
        {
            if (this->contacts[windy::CollisionContact::Down]) {

                this->ignoreGravity = false;

                auto fallDelay = cocos2d::DelayTime::create(this->sprite->getActionDuration("fall"));

                auto setup = cocos2d::CallFunc::create([this]() {
                    this->sprite->runAction("fall");
                });

                auto morphDelayA = cocos2d::DelayTime::create(this->sprite->getActionDuration("morph_a"));

                auto morphA = cocos2d::CallFunc::create([this]() {
                    this->sprite->runAction("morph_a");
                });

                auto onEnd = cocos2d::CallFunc::create([this]() {
                    this->_attackState = AttackState::Attacking;
                });

                auto sequence = cocos2d::Sequence::create(setup, fallDelay, morphA, morphDelayA, onEnd, nullptr);

                stopAllActions();

                runAction(sequence);

                _attackState = AttackState::Waiting;
            }
            else {
                this->speed.y = this->level->physicsWorld->gravity * 3;
            }
            
        }
        break;

        case AttackState::Idle:
        {
            auto delay = cocos2d::DelayTime::create(0.5f);

            auto callback = cocos2d::CallFunc::create([this]() {
                _attackState = AttackState::Attacking;
            });

            auto morphDelayB = cocos2d::DelayTime::create(this->sprite->getActionDuration("morph_b"));

            auto morphB = cocos2d::CallFunc::create([this]() {
                    this->sprite->runAction("morph_b");
            });


            auto sequence = cocos2d::Sequence::create(delay, morphB, morphDelayB, callback, nullptr);

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

            auto morphDelay = cocos2d::DelayTime::create(this->sprite->getActionDuration("morph_a_reversed"));

            auto morph = cocos2d::CallFunc::create([this]() {
                this->sprite->runAction("morph_a_reversed");
            });

            auto onEnd = cocos2d::CallFunc::create([this]() {
                this->_attackState = AttackState::Idle;
            });

            auto sequence = cocos2d::Sequence::create(attack, delay, delay, fireWeapon, morph, morphDelay, onEnd, nullptr);
            stopAllActions();
            runAction(sequence);
            _attackState = AttackState::Waiting;

        }
        break;
    }
}
