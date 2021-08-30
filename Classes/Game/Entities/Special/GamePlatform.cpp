#include <cmath>
#include <string>
#include <iostream>

#include "GamePlatform.h"
#include "Windy/GameTags.h"

#include "Windy/GeometryExtensions.h"

#include "Windy/Level.h"
#include "Windy/PhysicsWorld.h"
#include "Windy/Entities/Player.h"
#include "Windy/Entities/Camera.h"

using namespace game;

game::Resources& GamePlatform::getResources() {
    static game::Resources resources{ game::ResourceKind::LevelSpecial, "platform" };
    return resources;
}

bool GamePlatform::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    _radius = 16;
    _direction = 1;

    _isPlayerStanding = false;

    _centerPosition = getPosition();

    Logical::composite<GamePlatform>(this);

    this->setTag(windy::GameTags::General::Platform);

    return true;
}


void GamePlatform::parseBehavior(const cocos2d::ValueMap& behavior) {

    if (behavior.find("radius") != behavior.end()) {
        _radius = behavior.at("radius").asInt();
    }
    else {
        _radius = 16;
    }

    if (behavior.find("direction") != behavior.end()) {
        std::string direction = behavior.at("direction").asString();

        if (direction.compare("right") == 0) {
            _direction = 1;
        }
        else {
            _direction = -1;
        }
    }
    else {
        _direction = 1;
    }

    if (_direction == 1) {
        setPosition(_centerPosition + cocos2d::Point(static_cast<float>(_radius * -1), 0));
    }
    else {
        setPosition(_centerPosition + cocos2d::Point(static_cast<float>(_radius), 0));
    }

}

std::shared_ptr<cocos2d::Rect> GamePlatform::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<GamePlatform>(position, size);
}

void GamePlatform::onCollisionEnter(windy::Logical* collision) {
    if (collision->getTag() == windy::GameTags::Player) {
        if (windy::PhysicsWorld::getCollisionResult(collision, this)[windy::CollisionContact::Down]) {
            this->level->player->ignoreGravity = true;
            this->level->player->ignoreLandscapeCollision = true;

            this->level->player->speed.y = 0;

            this->level->player->autoControlled = true;

            //this->level->physicsWorld->alignCollisions(this->level->player, this, false);

            //this->level->player->recomputeCollisionRectangles();
            //this->level->player->recomputeCollisionRectangles();

            this->level->player->setPositionY(this->getPositionY() + this->collisionBox->size.height * 0.5f + this->level->player->collisionBox->size.height * 0.5f - 1);
            //this->level->player->onGround = true;
            this->level->player->recomputeCollisionRectangles();
            this->level->player->recomputeCollisionRectangles();


            //this->setTag(windy::GameTags::None);
            _isPlayerStanding = true;

            //this->onUpdate(1.0f / 60.0f);
        }
        
    }
}

void GamePlatform::onCollision(windy::Logical* collision) {
    if (collision->getTag() == windy::GameTags::Player) {
        
        if (collision->collisionBox->getMinY() >= this->collisionBox->getMaxY() - 2) {
            _isPlayerStanding = true;
            this->level->player->ignoreGravity = true;
            this->level->player->ignoreLandscapeCollision = true;

            this->level->player->autoControlled = true;

            //this->level->player->speed.y = 0;
        }
        else {
            this->level->physicsWorld->alignCollisions(this->level->player, this, false);
        }
        
        /*if (_isPlayerStanding) {
            //this->level->physicsWorld->alignCollisions(this->level->player, this, false);
        }*/
    }
}

void GamePlatform::onCollisionExit(windy::Logical* collision) {
    if (collision->getTag() == windy::GameTags::Player) {
        this->level->player->ignoreGravity = false;
        this->level->player->ignoreLandscapeCollision = false;
        //this->setTag(windy::GameTags::Block);
        _isPlayerStanding = false;
        this->level->player->autoControlled = false;

    }
}

void GamePlatform::onUpdate(float dt) {

    auto currentPosition = getPosition();
    auto position = getPosition();

    float s = std::sinf(windy::GeometryExtensions::degreesToRadians(1) * _direction);
    float c = std::cosf(windy::GeometryExtensions::degreesToRadians(1) * _direction);

    position.x = position.x - _centerPosition.x;
    position.y = position.y - _centerPosition.y;

    auto newPosition = cocos2d::Point(position.x * c - position.y * s, position.x * s + position.y * c);

    auto newPositionNoTranslation = newPosition;

    newPosition.x = newPosition.x + _centerPosition.x;
    newPosition.y = newPosition.y + _centerPosition.y;

    setPosition(newPosition);

    this->recomputeCollisionRectangles();
    this->recomputeCollisionRectangles();

    if (_isPlayerStanding && this->level->player->alive) {

        auto playerPosition = this->level->player->getPosition();

        this->level->player->setPositionX(playerPosition.x + (newPosition.x - currentPosition.x));
        this->level->player->setPositionY(newPosition.y + this->collisionBox->size.height * 0.5f + this->level->player->collisionBox->size.height * 0.5f - 1);
        this->level->player->contacts[windy::CollisionContact::Down] = true;
        //this->level->player->onGround = true;
        this->level->player->recomputeCollisionRectangles();
        this->level->player->recomputeCollisionRectangles();


        //this->level->physicsWorld->alignCollisions(this->level->player, this, false);
    }
    else if(!this->level->player->alive){
        if (_isPlayerStanding) {
            _isPlayerStanding = false;

            this->level->player->ignoreEntityCollision = true;
            this->level->physicsWorld->unregisterContact(this, this->level->player);
        }        
    }
    
}