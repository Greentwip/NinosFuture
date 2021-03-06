#include <cmath>

#include "PhysicsWorld.h"

#include "Level.h"

#include "Entities/Logical.h"
#include "Entities/Bounds.h"

#include "GameTags.h"

#include "GeometryExtensions.h"

using namespace windy;

PhysicsWorld* PhysicsWorld::create(Level* level) {

    PhysicsWorld* physicsWorld = new (std::nothrow) PhysicsWorld();

    if (physicsWorld) {
        physicsWorld->level = level;
    }

    if (physicsWorld && physicsWorld->init()) {
        physicsWorld->autorelease();
        physicsWorld->contactEventCollisionIndex = 0;
        return physicsWorld;
    }

    CC_SAFE_DELETE(physicsWorld);
    return nullptr;
}

bool PhysicsWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->gravity = -0.25;
    this->maxFallSpeed = -6;

    return true;
}

void PhysicsWorld::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 1024, false);
}


void PhysicsWorld::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void PhysicsWorld::setUnderwater(bool underwater) {
    if (underwater) {
        this->gravity = -0.125;
        this->maxFallSpeed = -3;
    }
    else {
        this->gravity = -0.25;
        this->maxFallSpeed = -6;
    }
}

std::map<CollisionContact, bool> PhysicsWorld::getCollisionResult(Logical* entity, Logical* landscapeEntity) {

    std::map<CollisionContact, bool> result;

    auto entityPosition = entity->getPosition();
    auto lastEntityCollisionPosition = entity->getPosition();

    for (int k = 0; k < 8; k++) {

        auto landscapeEntityCollisionBox = landscapeEntity->collisionBox;

        float collisionBoxLeft = entity->collisionBox->getMinX();
        float collisionBoxTop = entity->collisionBox->getMaxY();

        float collisionBoxWidth = entity->collisionBox->size.width;
        float collisionBoxHeight = entity->collisionBox->size.height;

        auto collisionBoxCenter = cocos2d::Point(entity->collisionBox->getMidX(), entity->collisionBox->getMidY());

        auto entityCollisionBox = *entity->collisionBox;

        /*
        * Collision tiles and sliced collision box
        ------------------
        -- 0 -- 1 -- 2 --
        -- 3 -- 4 -- 5 --
        -- 6 -- 7 -- 8 --
        ------------------

        ------------------
        -- 4 -- 1 -- 5 --
        -- 2 --   -- 3 --
        -- 6 -- 0 -- 7 --
        ------------------

        */

        cocos2d::Rect collisionBoxTiles[9];

        for (int j = 0; j < 9; ++j) {
            int column = j % 3;
            int row = static_cast<int>(std::floor(j / 3));

            float tileW = entityCollisionBox.size.width / 3.0f;
            float tileH = entityCollisionBox.size.height / 3.0f;

            float collisionBoxLeft = entityCollisionBox.getMinX();
            float collisionBoxTop = entityCollisionBox.getMaxY();

            auto tileOrigin = cocos2d::Point(collisionBoxLeft + (tileW * column), collisionBoxTop - (tileH * row));

            collisionBoxTiles[j] = cocos2d::Rect(tileOrigin.x, tileOrigin.y - tileH, tileW, tileH);

        }


        cocos2d::Rect slicedCollisionBox[8];

        slicedCollisionBox[0] = collisionBoxTiles[7];
        slicedCollisionBox[1] = collisionBoxTiles[1];
        slicedCollisionBox[2] = collisionBoxTiles[3];
        slicedCollisionBox[3] = collisionBoxTiles[5];
        slicedCollisionBox[4] = collisionBoxTiles[0];
        slicedCollisionBox[5] = collisionBoxTiles[2];
        slicedCollisionBox[6] = collisionBoxTiles[6];
        slicedCollisionBox[7] = collisionBoxTiles[8];

        if (landscapeEntityCollisionBox->intersectsRect(slicedCollisionBox[k])) {
            auto intersection = GeometryExtensions::rectIntersection(*landscapeEntityCollisionBox, slicedCollisionBox[k]);

            bool hasOffsetX = false;
            bool hasOffsetY = false;

            if (k == 0) {
                if (intersection.size.height != 0) {
                    result[CollisionContact::Down] = true;
                }
                hasOffsetY = true;
            }
            else if (k == 1)
            {
                if (intersection.size.height != 0) {
                    result[CollisionContact::Up] = true;
                }

                hasOffsetY = true;
                intersection.size.height *= -1;
            }
            else if (k == 2) {
                if (intersection.size.width != 0) {
                    result[CollisionContact::Left] = true;
                }
                hasOffsetX = true;
            }
            else if (k == 3) {
                
                if (intersection.size.width != 0) {
                    result[CollisionContact::Right] = true;
                }
                hasOffsetX = true;
                intersection.size.width *= -1;
            }
            else {

                if (intersection.size.width >= intersection.size.height) {
                    // Tile is diagonal, but resolving collision vertically
                    if (k == 4 || k == 5) {
                        intersection.size.height = -intersection.size.height;
                        if (intersection.size.height != 0) {
                            result[CollisionContact::Up] = true;
                        }
                        
                    }
                    else {
                        if (intersection.size.height != 0) {
                            result[CollisionContact::Down] = true;
                        }
                        
                    }

                    hasOffsetY = true;

                }
                else {
                    // Tile is diagonal, but resolving horizontally
                    if (k == 7 || k == 5) {
                        if (intersection.size.width != 0) {
                            result[CollisionContact::Right] = true;
                        }
                        
                        intersection.size.width *= -1;
                    }
                    else {
                        if (intersection.size.width != 0) {
                            result[CollisionContact::Left] = true;
                        }
                        
                    }

                    hasOffsetX = true;

                    // Only if slope is higher than the slope factor

                    if (k == 6 || k == 7) {
                        if (intersection.size.height <= 1.0f) {
                            result[CollisionContact::Left] = false;
                            result[CollisionContact::Right] = false;
                            intersection.size.height = 0;
                            hasOffsetY = true;
                        }

                    }



                }
            }

            if (hasOffsetX) {
                entityPosition.x = entityPosition.x + intersection.size.width;
            }

            if (hasOffsetY) {
                entityPosition.y = entityPosition.y + intersection.size.height;
            }

            auto positionDifference = entityPosition - lastEntityCollisionPosition;
            lastEntityCollisionPosition = entityPosition;

            float differenceX = positionDifference.x;
            float differenceY = positionDifference.y;

            entityCollisionBox.origin.x += differenceX;
            entityCollisionBox.origin.y += differenceY;

        }
    }

    return result;
}

void PhysicsWorld::alignCollisions(Logical* entity, Logical* landscapeEntity, bool clearContacts) {

    if (clearContacts) {
        entity->contacts.clear();
    }

    for (int k = 0; k < 8; k++) {

        auto landscapeEntityCollisionBox = landscapeEntity->collisionBox;

        float collisionBoxLeft = entity->collisionBox->getMinX();
        float collisionBoxTop = entity->collisionBox->getMaxY();

        float collisionBoxWidth = entity->collisionBox->size.width;
        float collisionBoxHeight = entity->collisionBox->size.height;

        auto collisionBoxCenter = cocos2d::Point(entity->collisionBox->getMidX(), entity->collisionBox->getMidY());


        /*
        * Collision tiles and sliced collision box
        ------------------
        -- 0 -- 1 -- 2 --
        -- 3 -- 4 -- 5 --
        -- 6 -- 7 -- 8 --
        ------------------

        ------------------
        -- 4 -- 1 -- 5 --
        -- 2 --   -- 3 --
        -- 6 -- 0 -- 7 --
        ------------------

        */

        cocos2d::Rect collisionBoxTiles[9];

        for (int j = 0; j < 9; ++j) {
            int column = j % 3;
            int row = static_cast<int>(std::floor(j / 3));

            float tileW = entity->collisionBox->size.width / 3.0f;
            float tileH = entity->collisionBox->size.height / 3.0f;

            float collisionBoxLeft = entity->collisionBox->getMinX();
            float collisionBoxTop = entity->collisionBox->getMaxY();

            auto tileOrigin = cocos2d::Point(collisionBoxLeft + (tileW * column), collisionBoxTop - (tileH * row));

            collisionBoxTiles[j] = cocos2d::Rect(tileOrigin.x, tileOrigin.y - tileH, tileW, tileH);

        }


        cocos2d::Rect slicedCollisionBox[8];

        slicedCollisionBox[0] = collisionBoxTiles[7];
        slicedCollisionBox[1] = collisionBoxTiles[1];
        slicedCollisionBox[2] = collisionBoxTiles[3];
        slicedCollisionBox[3] = collisionBoxTiles[5];
        slicedCollisionBox[4] = collisionBoxTiles[0];
        slicedCollisionBox[5] = collisionBoxTiles[2];
        slicedCollisionBox[6] = collisionBoxTiles[6];
        slicedCollisionBox[7] = collisionBoxTiles[8];

        if (landscapeEntityCollisionBox->intersectsRect(slicedCollisionBox[k])) {
            auto intersection = GeometryExtensions::rectIntersection(*landscapeEntityCollisionBox, slicedCollisionBox[k]);

            bool hasOffsetX = false;
            bool hasOffsetY = false;

            if (k == 0) {
                entity->contacts[CollisionContact::Down] = true;

                hasOffsetY = true;
            }
            else if (k == 1)
            {
                entity->contacts[CollisionContact::Up] = true;

                hasOffsetY = true;
                intersection.size.height *= -1;
            }
            else if (k == 2) {
                entity->contacts[CollisionContact::Left] = true;

                hasOffsetX = true;
            }
            else if (k == 3) {
                entity->contacts[CollisionContact::Right] = true;

                hasOffsetX = true;
                intersection.size.width *= -1;
            }
            else {

                if (intersection.size.width >= intersection.size.height) {
                    // Tile is diagonal, but resolving collision vertically
                    if (k == 4 || k == 5) {
                        intersection.size.height = -intersection.size.height;
                        if (intersection.size.height != 0) {
                            entity->contacts[CollisionContact::Up] = true;
                        }
                        
                    }
                    else {
                        if (intersection.size.height != 0) {
                            entity->contacts[CollisionContact::Down] = true;
                        }
                        
                    }

                    hasOffsetY = true;

                }
                else {
                    // Tile is diagonal, but resolving horizontally
                    if (k == 7 || k == 5) {
                        if (intersection.size.width != 0) {
                            entity->contacts[CollisionContact::Right] = true;
                        }
                        
                        intersection.size.width *= -1;
                    }
                    else {
                        if (intersection.size.width != 0) {
                            entity->contacts[CollisionContact::Left] = true;
                        }
                    }

                    hasOffsetX = true;

                    // Only if slope is higher than the slope factor

                    if (k == 6 || k == 7) {
                        if (intersection.size.height <= 1.0f) {
                            entity->contacts[CollisionContact::Left] = false;
                            entity->contacts[CollisionContact::Right] = false;
                            intersection.size.height = 0;
                            hasOffsetY = true;
                        }

                    }



                }
            }

            if (hasOffsetX) {
                entity->setPositionX(entity->getPositionX() + intersection.size.width);
                entity->recomputeCollisionRectangles();
            }

            if (hasOffsetY) {
                entity->setPositionY(entity->getPositionY() + intersection.size.height);
                entity->recomputeCollisionRectangles();
            }
        }
    }
}

void PhysicsWorld::update(float dt)
{
    cocos2d::Vector<Logical*> landscapeEntities;
    cocos2d::Vector<Logical*> collidingEntities;

    for (int i = 0; i < this->level->entities.size(); ++i) {
        auto entity = this->level->entities.at(i);

        if (entity->getTag() == GameTags::General::Block)
        {
            landscapeEntities.pushBack(entity);
        }
        
        if(//entity->getTag() == GameTags::General::Block        ||
           entity->getTag() == GameTags::General::Player       ||
           entity->getTag() == GameTags::General::Enemy        ||
           entity->getTag() == GameTags::General::Boss         ||
           entity->getTag() == GameTags::General::Item         ||
           entity->getTag() == GameTags::General::Ladder       ||
           entity->getTag() == GameTags::General::Door         ||
           entity->getTag() == GameTags::General::Camera       ||
           entity->getTag() == GameTags::General::Scroll       ||
           entity->getTag() == GameTags::General::Hole         ||
           entity->getTag() == GameTags::General::Teleporter   ||
           entity->getTag() == GameTags::General::Bounds       ||
           entity->getTag() == GameTags::General::Checkpoint   ||
           entity->getTag() == GameTags::General::Platform     ||
           entity->getTag() == GameTags::General::Water        ||
           entity->getTag() == GameTags::Weapon::WeaponPlayer  ||
           entity->getTag() == GameTags::Weapon::WeaponEnemy   ||
           entity->getTag() == GameTags::Scenery::Particle) { 
            if (!entity->ignoreEntityCollision) {
                collidingEntities.pushBack(entity);
            }
            
        }
    }

    // Apply speed and gravity
    for (int i = 0; i < collidingEntities.size(); ++i) {
        auto entity = collidingEntities.at(i);

        auto entityPosition = entity->getPosition();

        if (!entity->ignoreGravity) {
            if (!this->level->getPaused()) {
                entity->speed.y += this->gravity;
            }
            

            if (entity->speed.y <= this->maxFallSpeed) {
                entity->speed.y = this->maxFallSpeed;
            }
        }

        if (!this->level->getPaused()) {
            entityPosition += entity->speed;
        }
        

        entity->setPosition(entityPosition);

        entity->recomputeCollisionRectangles();
    }


    for (int i = 0; i < collidingEntities.size(); ++i) {
        auto entity = collidingEntities.at(i);

        entity->contacts.clear();

        if (entity->ignoreLandscapeCollision) {
            continue;
        }

        for (int j = 0; j < landscapeEntities.size(); ++j) {
            auto landscapeEntity = landscapeEntities.at(j);

            if (entity == landscapeEntity || entity->getTag() == GameTags::Block) {
                continue;
            }

            PhysicsWorld::alignCollisions(entity, landscapeEntity);

        }
    }

    // Clamp speed
    for (int i = 0; i < collidingEntities.size(); ++i) {
        auto entity = collidingEntities.at(i);

        if (entity->contacts[CollisionContact::Down] || entity->contacts[CollisionContact::Up]) {
            entity->speed.y = 0;
        }

        if (entity->contacts[CollisionContact::Left] || entity->contacts[CollisionContact::Right]) {
            entity->speed.x = 0;
        }
    }

    if (this->level->getPaused()) {
        this->contactEventCollisions.clear();
        return;
    }

    for (int i = 0; i < collidingEntities.size(); ++i) {
        auto entity = collidingEntities.at(i);

        for (int j = 0; j < collidingEntities.size(); ++j) {
            auto collidingEntity = collidingEntities.at(j);

            if (entity == collidingEntity) {
                continue;
            }

            if (entity->ignorePlatforms && collidingEntity->getTag() == GameTags::Platform ||
                collidingEntity->ignorePlatforms && entity->getTag() == GameTags::Platform) {
                continue;
            }

            if (GeometryExtensions::rectIntersectsRect(*entity->collisionBox, *collidingEntity->collisionBox)) {
                
                if (entity->collisionMaskFlags.size() != 0) {
                    bool collisionMaskMatches = false;
                    for (int i = 0; i < entity->collisionMaskFlags.size(); ++i) {
                        int tag = entity->collisionMaskFlags[i];
                        if (tag == collidingEntity->getTag()) {
                            collisionMaskMatches = true;
                            break;
                        }
                    }

                    if (!collisionMaskMatches) {
                        continue;
                    }
                }
                


                auto iterator = 
                    std::find_if(this->contactEventCollisions.begin(),
                                 this->contactEventCollisions.end(), [=](const std::pair<long long, std::pair<Logical*, Logical*>> contact) {
                            return contact.second.first == entity && contact.second.second == collidingEntity;
                    });

                if (iterator == contactEventCollisions.end()) {

                    long long contactIndex = 0;
                    long long emptyIndex = 0;

                    bool indexFound = false;

                    for (long long k = 0; k <= this->contactEventCollisionIndex; ++k) {
                        
                        indexFound = false;

                        for (int l = 0; l < this->contactEventCollisions.size(); ++l) {
                            auto contact = this->contactEventCollisions.at(l);

                            if (contact.first == k) {
                                indexFound = true;
                                break;
                            }
                        }

                        if (!indexFound) {
                            emptyIndex = k;
                            break;
                        }

                    }

                    if (!indexFound) {
                        contactIndex = emptyIndex;
                    }
                    else {
                        contactIndex = this->contactEventCollisionIndex + 1;
                        this->contactEventCollisionIndex += 1;
                    }

                    this->contactEventCollisions.push_back({ contactIndex, { entity, collidingEntity } });
                    entity->onCollisionEnter(collidingEntity);

                }
            }
        }
    }


    this->contactEventCollisions.erase(
        std::remove_if(this->contactEventCollisions.begin(),
                       this->contactEventCollisions.end(),
                        [=](const std::pair<long long, std::pair<Logical*, Logical*>> contact) {
                bool entityExists = this->level->entities.find(contact.second.first) != this->level->entities.end();
                bool collisionExists = this->level->entities.find(contact.second.second) != this->level->entities.end();

                return !entityExists || !collisionExists;
        }),
        this->contactEventCollisions.end());

    std::vector<std::pair<long long, std::pair<Logical*, Logical*>>> contactExitCollisions;

    for (int i = 0; i < this->contactEventCollisions.size(); ++i) {
        auto contact = this->contactEventCollisions.at(i);

        auto entity = contact.second.first;
        auto collidingEntity = contact.second.second;


        long long collisionId = contact.first;

        if (GeometryExtensions::rectIntersectsRect(*entity->collisionBox, *collidingEntity->collisionBox)) {

            entity->onCollision(collidingEntity);
        }
        else {
            contactExitCollisions.push_back(contact);
        }
    }

    this->contactEventCollisions.erase(
        std::remove_if(this->contactEventCollisions.begin(),
                       this->contactEventCollisions.end(),
                       [=](const std::pair<long long, std::pair<Logical*, Logical*>> contact) {

                bool shouldRemove = false;
                for (int i = 0; i < contactExitCollisions.size(); ++i) {
                    auto validationContact = contactExitCollisions.at(i);

                    shouldRemove = validationContact.first == contact.first && 
                                    validationContact.second.first == contact.second.first &&
                                    validationContact.second.second == contact.second.second;

                    if (shouldRemove) {
                        break;
                    }

                }

                return shouldRemove;
            }),
        this->contactEventCollisions.end());


    for (int i = 0; i < contactExitCollisions.size(); ++i) {
        auto contact = contactExitCollisions.at(i);

        auto entity = contact.second.first;
        auto collidingEntity = contact.second.second;

        entity->onCollisionExit(collidingEntity);
    }

}

void PhysicsWorld::unregisterContact(Logical* a, Logical* b) {
    this->contactEventCollisions.erase(
        std::remove_if(this->contactEventCollisions.begin(),
                       this->contactEventCollisions.end(),
                       [=](const std::pair<long long, std::pair<Logical*, Logical*>> contact) {

                bool shouldRemove = false;
                shouldRemove = (contact.second.first == a &&
                    contact.second.second == b) ||
                    (contact.second.first == b &&
                        contact.second.second == a);

                return shouldRemove;
            }),
        this->contactEventCollisions.end());

}

void PhysicsWorld::unregisterContact(Logical* a) {
    this->contactEventCollisions.erase(
        std::remove_if(this->contactEventCollisions.begin(),
            this->contactEventCollisions.end(),
            [=](const std::pair<long long, std::pair<Logical*, Logical*>> contact) {

                bool shouldRemove = false;
                shouldRemove = (contact.second.first == a || contact.second.second == a);

                if (shouldRemove) {
                    contact.second.first->onCollisionExit(contact.second.second);
                }

                return shouldRemove;
            }),
        this->contactEventCollisions.end());

}


void PhysicsWorld::resetContactEventCollisions() {
    this->contactEventCollisions.clear();
}
