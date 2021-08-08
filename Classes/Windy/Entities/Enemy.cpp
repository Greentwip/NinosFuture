#include <memory>
#include <string>

#include "Enemy.h"

#include "Player.h"
#include "Weapon.h"

#include "./../Armature.h"
#include "./../AnimationAction.h"



#include "./../GameTags.h"
#include "./../Sprite.h"

using namespace windy;

std::random_device Enemy::itemRandomDevice;
std::default_random_engine  Enemy::itemRandomEngine(Enemy::itemRandomDevice());

void Enemy::composite(Enemy* enemy, const std::string& armaturePath, const std::string& spritePath, const std::string& enemyName) {

    auto armature = Armature(armaturePath);

    auto newAnchor = armature.get(enemyName).anchor;

    enemy->sprite = Sprite::create(spritePath, newAnchor);
    enemy->addChild(enemy->sprite);

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = enemy->sprite->getContentSize();

    enemy->collisionRectangles = armature.get(enemyName).collisionRectangles;

    //auto collisionBoxCenter = cocos2d::Point(enemy->collisionRectangles[0]->getMidX(), enemy->collisionRectangles[0]->getMidY());

    for (int i = 0; i < enemy->collisionRectangles.size(); ++i) {
        enemy->collisionRectangles[i] = Logical::normalizeCollisionRectangle(enemy, *enemy->collisionRectangles[i]);
    }

    enemy->collisionBox = enemy->collisionRectangles[0];


    //enemy->sprite->setPosition(collisionBoxCenter + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));
    enemy->sprite->setPosition(cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));
}

std::shared_ptr<cocos2d::Rect> Enemy::buildEntryCollisionRectangle(const cocos2d::Point& position,
                                                                   const cocos2d::Size& size,
                                                                   const std::string& armaturePath,
                                                                   const std::string& enemyName)
{
    auto armature = windy::Armature(armaturePath);

    auto newAnchor = armature.get(enemyName).anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get(enemyName).collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = windy::Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}


bool Enemy::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->power = 4;
    this->maxHealth = 1;
    this->health = this->maxHealth;

    this->setup();

    this->setTag(GameTags::General::Enemy);

    return true;
}


void Enemy::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Enemy::setOrientation() {
    if (this->level->player->getPositionX() <= this->getPositionX()) {
        this->sprite->setFlippedX(false);
    }
    else {
        this->sprite->setFlippedX(true);
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

        auto weapon = dynamic_cast<Weapon*>(collision);

        this->health -= weapon->power;

        /*if (this->health <= 0) {
            this->onDefeated();
        }*/

        if (weapon->power < 3) {
            weapon->finish();
        }


    }


}

void Enemy::onUpdate(float dt) {
    this->setOrientation();
    this->attack();
    this->checkHealth();
}