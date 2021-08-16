#include "Tremor.h"

#include "TremorTail.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/EntityFactory.h"

#include "Windy/ObjectManager.h"

#include "Windy/Entities/Door.h"

#include "Game/GameManager.h"

#include "Game/Entities/Weapons/DIrectionalBullet.h"

#include "Game/Entities/Player/GamePlayer.h"

#include "Game/Entities/Weapons/TremorDrill.h"

#include "Game/Entities/Weapons/TremorLaser.h"

#include "Game/Entities/Items/GameItem.h"





using namespace game;


class TremorResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string TremorResources::spritePath = "sprites/characters/enemy/sheriff/tremor/tremor";
std::string TremorResources::armaturePath = "physics/characters/enemy/sheriff/tremor/tremor";

void Tremor::preloadResources() {
    windy::Armature::cache(TremorResources::armaturePath);
    windy::Sprite::cache(TremorResources::spritePath);
}

void Tremor::setup() {

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->maxHealth = 5;
    this->health = this->maxHealth;

    this->power = 10;

    this->startPosition = this->getPosition();
    
    this->attackState = AttackState::None;

    Logical::composite<windy::Enemy>(this, TremorResources::armaturePath, TremorResources::spritePath, "tremor");


    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("head",      "tremor_head",      false,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);
    this->sprite->setAnimation("tremor_head");
    this->sprite->runAction("head");
    this->sprite->reverseAction();


    float offsetY = this->sprite->getPositionY();

    float offsetX = 5;

    for (int i = 0; i < 5; ++i) {
        auto bodySprite = windy::Sprite::create(TremorResources::spritePath);
        bodySprite->setAnimation("tremor_body");

        this->addChild(bodySprite, -32 - i);

        offsetY = offsetY - bodySprite->getContentSize().height * 0.75f;

        bodySprite->setPosition(cocos2d::Point(offsetX, offsetY));
    }
        

    this->tail = TremorTail::create();
    this->level->addChild(this->tail);

    this->level->entities.pushBack(this->tail->drill);

    this->tail->setPosition(this->getPositionX(), this->getPositionY() - 256);

    this->tail->drill->normalizeCollisionRectangles();

    offsetX = 0;
    offsetY = -2;

    for (int i = 0; i < 5; ++i) {
        auto tailBody = windy::Sprite::create(TremorResources::spritePath);
        tailBody->setAnimation("tremor_body");

        this->tail->addChild(tailBody, -32 - i);

        offsetY = offsetY - tailBody->getContentSize().height * 0.75f;

        tailBody->setPosition(cocos2d::Point(offsetX, offsetY));
    }

    auto horizontalDoor = this->level->horizontalDoors.at(0);
    horizontalDoor->close(false);
}


std::shared_ptr<cocos2d::Rect> Tremor::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, TremorResources::armaturePath, "tremor");
}


void Tremor::setOrientation() {
}

void Tremor::onFinished() {
    this->level->entities.eraseObject(this->tail->drill);
    this->tail->removeFromParent();
}



void Tremor::onDefeated() {

    windy::AudioManager::playSfx(windy::Sounds::Explosion1);

    std::string item = "";

    if (GameManager::getInstance().unlockables.head->acquired) {
        item = "health_big";
    }
    else {
        item = "head";
    }

    auto entryCollisionBox = Logical::getEntryCollisionRectangle<GameItem>(this->getPosition(), cocos2d::Size(16, 16));

    auto itemPosition = this->getPosition();
    auto entry = Logical::getEntry(entryCollisionBox, [=]() {

        auto itemGood = windy::EntityFactory::getInstance().create("item", itemPosition, cocos2d::Size(16, 16));

        dynamic_cast<GameItem*>(itemGood)->setup("none", item, true, false);

        return itemGood;
        });

    this->level->objectManager->objectEntries.push_back(entry);

    auto horizontalDoor = this->level->horizontalDoors.at(0);

    this->level->horizontalDoors.at(0)->unlock([horizontalDoor]() {
        horizontalDoor->setTraversable(false);
    });
    

    this->finish();

}

void Tremor::onCannonAttackEnd() {
    this->sprite->reverseAction();

    this->tail->drill->sprite->runAction("tail_idle");

    auto playerXY = cocos2d::MoveTo::create(1, this->level->player->getPosition());

    auto screenXY = cocos2d::MoveTo::create(1, cocos2d::Point(this->getPositionX(), this->getPositionY() - 256));

    auto delay = cocos2d::DelayTime::create(this->tail->drill->sprite->getActionDuration("tail_a") * 8);
    auto halfDelay = cocos2d::DelayTime::create(this->tail->drill->sprite->getActionDuration("tail_a") * 4);

    auto sequence = cocos2d::Sequence::create(
        playerXY,
        cocos2d::CallFunc::create([this]() {
            this->tail->drill->sprite->runAction("tail_a");
        }),
        halfDelay,
        cocos2d::CallFunc::create([this]() {
            this->tail->drill->enableAttack();
        }),
        cocos2d::CallFunc::create([this]() {
            this->tail->drill->sprite->runAction("tail_b");
        }),
        delay,
        cocos2d::CallFunc::create([this]() {
            this->tail->drill->disableAttack();
        }),
        cocos2d::CallFunc::create([this]() {
            this->tail->drill->sprite->runAction("tail_c");
        }),
        delay,
        screenXY,
        nullptr);


    this->tail->stopAllActions();
    this->tail->runAction(sequence);

    auto reinit = [this]() {
        this->attackState = AttackState::None;
    };

    auto moveToStart = cocos2d::MoveTo::create(2, this->startPosition);
    auto moveDelay = cocos2d::DelayTime::create(1);

    sequence = cocos2d::Sequence::create(moveToStart, moveDelay, cocos2d::CallFunc::create(reinit), nullptr);

    this->runAction(sequence);

}

void Tremor::attack(float dt) {
    
    switch (this->attackState) {
        case AttackState::None:
            this->attackState = AttackState::Moving;
        break;

        case AttackState::Moving: {
            auto moveDown = cocos2d::MoveTo::create(1.5f, cocos2d::Point(this->getPositionX(), this->getPositionY() - 128));
            auto moveUp = cocos2d::MoveTo::create(1.5f, cocos2d::Point(this->getPositionX(), this->getPositionY() + 24));
            auto moveDelay = cocos2d::DelayTime::create(1);

            auto sequence =
                cocos2d::Sequence::create(
                    moveDown, 
                    moveDelay,
                    moveUp, 
                    moveDelay, 
                    cocos2d::CallFunc::create([this]() { 
                        this->attackState = AttackState::Attacking;
                    }), nullptr);

            this->runAction(sequence);

            this->attackState = AttackState::Working;
        }
        break;

        case AttackState::Working: {
        }
        break;

        case AttackState::Attacking:{
            auto locatePlayerY = cocos2d::MoveTo::create(1, cocos2d::Point(this->getPositionX(), this->level->player->getPositionY()));

            auto animateAttack = cocos2d::CallFunc::create([this]() {
                this->sprite->reverseAction();
                });

            auto fireCallback = cocos2d::CallFunc::create([this]() {
                auto offset = cocos2d::Point(-72, -10);

                auto bulletPosition = this->getPosition() + offset;

                auto entryCollisionBox = TremorLaser::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 16));

                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    auto bullet = TremorLaser::create();
                    bullet->setPosition(bulletPosition);
                    bullet->setup();

                    bullet->fire(8, -this->getSpriteNormal(), windy::GameTags::WeaponEnemy);

                    return bullet;
                    });

                this->level->objectManager->objectEntries.push_back(entry);
                });

            auto attackDelay = cocos2d::DelayTime::create(2);

            auto attackEndCallback = cocos2d::CallFunc::create([this]() {
                this->onCannonAttackEnd();
                });

            auto sequence = cocos2d::Sequence::create(locatePlayerY, animateAttack, fireCallback, attackDelay, attackEndCallback, nullptr);

            this->runAction(sequence);

            this->attackState = AttackState::Working;
        }
        break;
    }
}
