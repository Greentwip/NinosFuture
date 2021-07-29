#include <memory>
#include <string>

#include "Enemy.h"

#include "Player.h"
#include "Item.h"

#include "Bullet.h"

#include "DirectionalBullet.h"

#include "ObjectEntry.h"

#include "./../GameTags.h"
#include "./../Armature.h"
#include "./../AnimationAction.h"
#include "./../Sprite.h"

#include "./../Level.h"
#include "./../ObjectManager.h"

#include "./../EntityFactory.h"



using namespace windy;


class CannonJoeResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string CannonJoeResources::spritePath = "sprites/characters/enemy/general/cannon_joe/cannon_joe";
std::string CannonJoeResources::armaturePath = "physics/characters/enemy/general/cannon_joe/cannon_joe";


std::random_device Enemy::itemRandomDevice;
std::default_random_engine  Enemy::itemRandomEngine(Enemy::itemRandomDevice());

bool Enemy::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->power = 4;
    this->health = 15;
    this->maxHealth = 15;

    this->attacking = false;
    this->isFlipping = false;

    this->attackTimer = 0;

    this->attackTimeInterval = 60;

    this->bulletPower = 3;

    this->attackState = AttackState::None;


    auto armature = Armature(CannonJoeResources::armaturePath);

    auto newAnchor = armature.get("cannon_joe").anchor;

    this->sprite = Sprite::create(CannonJoeResources::spritePath, newAnchor);
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


    std::vector<AnimationAction> actionSet = {
        AnimationAction("stand",      "cannon_joe_stand",      true,   0.10f),
        AnimationAction("flip",       "cannon_joe_flip",       true,   0.24f),
        AnimationAction("shoot",      "cannon_joe_shoot",      true,   0.04f)
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("cannon_joe_stand");

    this->setTag(GameTags::General::Enemy);

    return true;
}


std::shared_ptr<cocos2d::Rect> Enemy::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {

    auto armature = Armature(CannonJoeResources::armaturePath);

    auto newAnchor = armature.get("cannon_joe").anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get("cannon_joe").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}



void Enemy::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Enemy::setOrientation() {
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


int Enemy::getSpriteNormal() {

    if (this->sprite->isFlippedX()) {
        return -1;
    }
    else {
        return 1;
    }
}

void Enemy::checkHealth() {
    if (this->health <= 0) {
        this->health = 0;
        this->onDefeated();
    }
}

void Enemy::onDefeated() {

    AudioManager::playSfx(Sounds::Explosion1);

    std::uniform_int_distribution<int> itemKindRandomDistribution(0, 400);
    std::uniform_int_distribution<int> itemVariantRandomDistribution(0, 1);
    int itemKind = itemKindRandomDistribution(Enemy::itemRandomEngine);

    std::string item = "";
    if (itemKind < 10) {
        item = "life";
    }
    else if (itemKind < 60) {
        std::string healthKind[2];
        healthKind[0] = "health_small";
        healthKind[1] = "health_big";
        int itemVariant = itemVariantRandomDistribution(Enemy::itemRandomEngine);
        item = healthKind[itemVariant];
    }
    else if (itemKind < 120) {
        std::string energyKind[2];
        energyKind[0] = "energy_small";
        energyKind[1] = "energy_big";
        int itemVariant = itemVariantRandomDistribution(Enemy::itemRandomEngine);
        item = energyKind[itemVariant];
    }

    if (item.compare("") != 0) {

        auto entryCollisionBox = Logical::getEntryCollisionRectangle<Enemy>(this->getPosition(), cocos2d::Size(16, 16));

        auto itemPosition = this->getPosition();
        auto entry = Logical::getEntry(entryCollisionBox, [=]() {

            auto itemGood = EntityFactory::getInstance().create("item", itemPosition, cocos2d::Size(16, 16));

            dynamic_cast<Item*>(itemGood)->setup(item, false);

            return itemGood;
        });

        this->level->objectManager->objectEntries.push_back(entry);
    }

    this->finish();

}

void Enemy::onCollisionEnter(Logical* collision) {

    if (collision->getTag() == GameTags::Weapon::WeaponPlayer) {
        
        
        AudioManager::playSfx(Sounds::EnemyHit);
        
        auto blink = cocos2d::Blink::create(0.2f, 4);
        auto callback = cocos2d::CallFunc::create([this]() {
            
            if (!this->sprite->isVisible()) {
                this->sprite->setVisible(true);
            }

        });

        auto sequence = cocos2d::Sequence::create(blink, callback, nullptr);

        ((cocos2d::Node*)this->sprite)->runAction(sequence);

        auto bullet = dynamic_cast<Bullet*>(collision);

        this->health -= bullet->power;

        if (this->health <= 0) {
            this->onDefeated();
        }

        if (bullet->power < 3) {
            bullet->finish();
        }


    }


}

void Enemy::attack() {
    switch(this->attackState){
        
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
                            bullet->fire(this->bulletPower, this->getSpriteNormal(), GameTags::WeaponEnemy);
                        }
                        else {
                            bullet->fire(this->bulletPower, playerPosition, GameTags::WeaponEnemy);
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


void Enemy::onUpdate(float dt) {
    attack();
    checkHealth();
}