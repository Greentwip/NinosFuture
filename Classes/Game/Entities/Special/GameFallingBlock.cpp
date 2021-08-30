#include <cmath>
#include <string>
#include <iostream>

#include "GameFallingBlock.h"
#include "Windy/GameTags.h"

#include "Windy/GeometryExtensions.h"

#include "Windy/Level.h"
#include "Windy/PhysicsWorld.h"
#include "Windy/EntityFactory.h"
#include "Windy/ObjectManager.h"
#include "Windy/Entities/Bounds.h"
#include "Windy/Entities/Player.h"


using namespace game;

game::Resources& GameFallingBlock::getResources() {
    static game::Resources resources{ game::ResourceKind::LevelSpecial, "falling_block" };
    return resources;
}

bool GameFallingBlock::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    _ready = true;
    _shouldFall = false;

    _isPlayerStanding = false;

    _originalPosition = getPosition();

    Logical::composite<GameFallingBlock>(this);

    this->setTag(windy::GameTags::General::Platform);

    return true;
}

void GameFallingBlock::reappear() {
    
    _ready = false;
    _shouldFall = false;

    auto blink = cocos2d::Blink::create(1, 2);

    auto blinkCallback = cocos2d::CallFunc::create([this]() {
        if (!this->isVisible()) {
            this->setVisible(true);
        }

        if (!this->sprite->isVisible()) {
            this->sprite->setVisible(true);
        }

        this->_ready = true;
    });

    auto sequence = cocos2d::Sequence::create(blink, blinkCallback, nullptr);

    sequence->setTag(windy::GameTags::Actions::Visibility);

    this->stopAllActionsByTag(windy::GameTags::Actions::Visibility);
    this->runAction(sequence);
    
}

void GameFallingBlock::onFinished() {

    auto entryCollisionBox = GameFallingBlock::getEntryCollisionRectangle(_originalPosition, cocos2d::Size(16, 16));

    if (windy::GeometryExtensions::rectIntersectsRect(*this->level->bounds->collisionBox, *entryCollisionBox)) {
        
        auto originalPosition = _originalPosition;
        auto entry = Logical::getEntry(entryCollisionBox, [=]() {
            auto entity =
                dynamic_cast<GameFallingBlock*>(
                    windy::EntityFactory::getInstance().create("falling_block", originalPosition, cocos2d::Size(16, 16), 16)
                );

            entity->reappear();

            return entity;
        });

        this->level->objectManager->objectEntries.push_back(entry);

        this->finishForever();

    }


}

std::shared_ptr<cocos2d::Rect> GameFallingBlock::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<GameFallingBlock>(position, size);
}

void GameFallingBlock::onCollisionEnter(windy::Logical* collision) {
    if (_ready) {
        if (collision->getTag() == windy::GameTags::Player) {
            if (windy::PhysicsWorld::getCollisionResult(collision, this)[windy::CollisionContact::Down]) {
                this->level->player->ignoreGravity = true;
                this->level->player->ignoreLandscapeCollision = true;

                this->level->player->speed.y = 0;

                this->level->player->setPositionY(this->getPositionY() + this->collisionBox->size.height * 0.5f + this->level->player->collisionBox->size.height * 0.5f - 1);

                //this->level->player->recomputeCollisionRectangles();
                //this->level->player->recomputeCollisionRectangles();


                _isPlayerStanding = true;

                if (!_shouldFall) {
                    _shouldFall = true;
                    auto delay = cocos2d::DelayTime::create(1.4f);
                    auto callback = cocos2d::CallFunc::create([this]() {
                        this->ignoreGravity = false;
                    });

                    auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);

                    this->runAction(sequence);
                }
            }

        }
        else if (collision->getTag() == windy::GameTags::Enemy || collision->getTag() == windy::GameTags::Item) {
            if (windy::PhysicsWorld::getCollisionResult(collision, this)[windy::CollisionContact::Down]) {
                if (_standingEntities.find(collision) == _standingEntities.end()) {
                    _standingEntities.pushBack(collision);

                    collision->ignoreGravity = true;
                    collision->ignoreLandscapeCollision = true;

                    collision->speed.y = 0;

                    collision->setPositionY(this->getPositionY() + this->collisionBox->size.height * 0.5f + collision->collisionBox->size.height * 0.5f - 1);

                }
                
            }
        }
    }
}

void GameFallingBlock::onCollision(windy::Logical* collision) {
    if (_ready) {
        if (collision->getTag() == windy::GameTags::Player) {

            if (collision->collisionBox->getMinY() >= this->collisionBox->getMaxY()) {
                _isPlayerStanding = true;
                this->level->player->ignoreGravity = true;
                this->level->player->ignoreLandscapeCollision = true;

                if (!_shouldFall) {
                    _shouldFall = true;
                    auto delay = cocos2d::DelayTime::create(1.4f);
                    auto callback = cocos2d::CallFunc::create([this]() {
                        this->ignoreGravity = false;
                    });

                    auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);

                    this->runAction(sequence);
                }

            }
            else {
                auto collisionResult = windy::PhysicsWorld::getCollisionResult(collision, this);
                if (collisionResult[windy::CollisionContact::Right] || 
                    collisionResult[windy::CollisionContact::Left] ||
                    collisionResult[windy::CollisionContact::Down]) {
                    this->level->physicsWorld->alignCollisions(this->level->player, this, false);
                }
                
            }
        }
        else if (collision->getTag() == windy::GameTags::Enemy || collision->getTag() == windy::GameTags::Item) {

            if (collision->collisionBox->getMinY() >= this->collisionBox->getMaxY()) {

                if (_standingEntities.find(collision) == _standingEntities.end()) {
                    _standingEntities.pushBack(collision);

                    collision->ignoreGravity = true;
                    collision->ignoreLandscapeCollision = true;

                }
                

            }
            else {
                auto collisionResult = windy::PhysicsWorld::getCollisionResult(collision, this);
                if (collisionResult[windy::CollisionContact::Right] ||
                    collisionResult[windy::CollisionContact::Left] ||
                    collisionResult[windy::CollisionContact::Down]) {
                    this->level->physicsWorld->alignCollisions(collision, this, false);
                }

            }
        }
    }
}

void GameFallingBlock::onCollisionExit(windy::Logical* collision) {
    if (_ready) {
        if (collision->getTag() == windy::GameTags::Player) {
            this->level->player->ignoreGravity = false;
            this->level->player->ignoreLandscapeCollision = false;

            _isPlayerStanding = false;
        }
        else if (collision->getTag() == windy::GameTags::Enemy) {
            if (_standingEntities.find(collision) != _standingEntities.end()) {
                collision->ignoreGravity = false;
                collision->ignoreLandscapeCollision = false;
                _standingEntities.eraseObject(collision);
            }
        }
    }
    
}

void GameFallingBlock::onUpdate(float dt) {

    if (_isPlayerStanding && this->level->player->alive) {
        
        this->level->player->setPositionY(this->getPositionY() + this->collisionBox->size.height * 0.5f + this->level->player->collisionBox->size.height * 0.5f);
        this->level->player->contacts[windy::CollisionContact::Down] = true;

        //this->level->player->recomputeCollisionRectangles();
        //this->level->player->recomputeCollisionRectangles();

    }
    else if (!this->level->player->alive) {
        if (_isPlayerStanding) {
            _isPlayerStanding = false;
            this->level->player->ignoreEntityCollision = true;
            this->level->physicsWorld->unregisterContact(this, this->level->player);
        }
    }

    for (int i = 0; i < _standingEntities.size(); ++i) {
        auto standingEntity = _standingEntities.at(i);

        standingEntity->setPositionY(this->getPositionY() + this->collisionBox->size.height * 0.5f + standingEntity->collisionBox->size.height * 0.5f);
        standingEntity->contacts[windy::CollisionContact::Down] = true;
    }
}