#include "CannonJoe.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/GameTags.h"

#include "Windy/ObjectManager.h"

#include "Game/Entities/Weapons/DIrectionalBullet.h"

#include "Game/Entities/Player/GamePlayer.h"



using namespace game;


class CannonJoeResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string CannonJoeResources::spritePath = "sprites/characters/enemy/general/cannon_joe/cannon_joe";
std::string CannonJoeResources::armaturePath = "physics/characters/enemy/general/cannon_joe/cannon_joe";



void CannonJoe::setup() {

    this->attacking = false;
    this->attackTimer = 0;
    this->attackTimeInterval = 60;
    this->bulletPower = 3;
    this->attackState = AttackState::None;


    auto armature = windy::Armature(CannonJoeResources::armaturePath);

    auto newAnchor = armature.get("cannon_joe").anchor;

    this->sprite = windy::Sprite::create(CannonJoeResources::spritePath, newAnchor);
    this->addChild(this->sprite);

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();

    this->collisionRectangles = armature.get("cannon_joe").collisionRectangles;

    auto collisionBoxCenter = cocos2d::Point(this->collisionRectangles[0]->getMidX(), this->collisionRectangles[0]->getMidY());

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];


    this->sprite->setPosition(collisionBoxCenter + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));


    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",      "cannon_joe_stand",      true,   0.10f),
        windy::AnimationAction("flip",       "cannon_joe_flip",       true,   0.24f),
        windy::AnimationAction("shoot",      "cannon_joe_shoot",      true,   0.04f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("cannon_joe_stand");
}


std::shared_ptr<cocos2d::Rect> CannonJoe::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {

    auto armature = windy::Armature(CannonJoeResources::armaturePath);

    auto newAnchor = armature.get("cannon_joe").anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get("cannon_joe").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = windy::Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}


void CannonJoe::setOrientation() {
    if (!this->isFlipping) {
        if (this->level->player->getPositionX() <= this->getPositionX()) {
            if (this->sprite->isFlippedX()) {
                this->sprite->setFlippedX(false);
                this->isFlipping = false;
            }
        }
        else {
            if (!this->sprite->isFlippedX()) {
                this->sprite->setFlippedX(true);
                this->isFlipping = false;
            }
        }
    }
}

void CannonJoe::attack() {
    switch (this->attackState) {

    case AttackState::None: {
        float playerDistanceX = cocos2d::Point(this->getPositionX(), 0).getDistance(cocos2d::Point(this->level->player->getPositionX(), 0));
        float playerDistanceY = cocos2d::Point(0, this->getPositionY()).getDistance(cocos2d::Point(0, this->level->player->getPositionY()));

        if (playerDistanceX > 48 || playerDistanceY <= 24) {
            this->attackState = AttackState::Before;
            this->attackTimer = this->attackTimeInterval;
        }

    }
                          break;

    case AttackState::Before: {
        if (this->attackTimer <= 0) {
            this->attackTimer = 0;

            float playerDistanceX = cocos2d::Point(this->getPositionX(), 0).getDistance(cocos2d::Point(this->level->player->getPositionX(), 0));
            float playerDistanceY = cocos2d::Point(0, this->getPositionY()).getDistance(cocos2d::Point(0, this->level->player->getPositionY()));


            if (playerDistanceX > 48 || playerDistanceY <= 24) {
                auto playerPosition = this->level->player->getPosition();

                auto bulletPosition = cocos2d::Point(this->getPositionX() + (22 * this->getSpriteNormal() * -1),
                    this->getPositionY());

                auto entryCollisionBox = DirectionalBullet::getEntryCollisionRectangle(bulletPosition, cocos2d::Size(16, 16));
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    auto bullet = DirectionalBullet::create();
                    bullet->setPosition(bulletPosition);
                    bullet->setup();

                    if (playerDistanceX < 30 && playerDistanceY < 16) {
                        bullet->fire(this->bulletPower, this->getSpriteNormal(), windy::GameTags::WeaponEnemy);
                    }
                    else {
                        bullet->fire(this->bulletPower, playerPosition, windy::GameTags::WeaponEnemy);
                    }


                    return bullet;
                    });



                this->level->objectManager->objectEntries.push_back(entry);

                this->attackTimer = this->attackTimeInterval;
                this->attackState = AttackState::Cooldown;
            }

        }
        else {
            this->attackTimer -= 1;
        }


    }
                            break;

    case AttackState::Cooldown:
    {
        if (this->attackTimer <= 0) {
            this->attackTimer = 0;
            this->attackState = AttackState::None;
        }
        else {
            this->attackTimer -= 1;
        }


    }
    break;

    }
}
