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
    return true;
}

void PhysicsWorld::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void PhysicsWorld::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
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
        else if(entity->getTag() == GameTags::General::Player ||
                entity->getTag() == GameTags::General::Enemy) { // Kinematic characters first
            collidingEntities.pushBack(entity);
        }
    }

    cocos2d::Vector<Logical*> screenLandscapeEntities;

    for (int i = 0; i < landscapeEntities.size(); ++i) {
        auto entity = landscapeEntities.at(i);

        auto landscapeCollisionBox = entity->collisionBox;
        auto boundsCollisionBox = this->level->bounds->collisionBox;

        if (boundsCollisionBox->intersectsRect(*landscapeCollisionBox)) {
            screenLandscapeEntities.pushBack(entity);
        }
    }

    // Apply gravity
    for (int i = 0; i < collidingEntities.size(); ++i) {
        auto entity = collidingEntities.at(i);
        entity->setPositionY(entity->getPositionY() - 0.5f);
    }


    for (int i = 0; i < collidingEntities.size(); ++i) {
        auto entity = collidingEntities.at(i);

        /*
        * Collision tiles and sliced collision box
        ------------------
        -- 1 -- 2 -- 3 --
        -- 4 -- 5 -- 6 --
        -- 7 -- 8 -- 9 --
        ------------------

        ------------------
        -- 5 -- 2 -- 6 --
        -- 3 --   -- 4 --
        -- 7 -- 1 -- 8 --
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


        for (int j = 0; j < screenLandscapeEntities.size(); ++j) {
            auto landscapeEntity = screenLandscapeEntities.at(j);
            auto landscapeEntityCollisionBox = landscapeEntity->collisionBox;

            float collisionBoxLeft = entity->collisionBox->getMinX();
            float collisionBoxTop = entity->collisionBox->getMaxY();

            float collisionBoxWidth = entity->collisionBox->size.width;
            float collisionBoxHeight = entity->collisionBox->size.height;

            auto collisionBoxCenter = cocos2d::Point(entity->collisionBox->getMidX(), entity->collisionBox->getMidY());

            for (int k = 0; k < 8; k++) {
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
                                entity->contacts[CollisionContact::Up] = true;
                            }
                            else { 
                                // 6 and 7
                                entity->contacts[CollisionContact::Down] = true;
                            }

                            hasOffsetY = true;

                        }
                        else {
                            // Tile is diagonal, but resolving horizontally
                            if (k == 6 || k == 5) {
                                entity->contacts[CollisionContact::Left] = true;

                            }
                            else {
                                entity->contacts[CollisionContact::Right] = true;
                                intersection.size.width *= -1;
                            }

                            hasOffsetX = true;

                        }
                    }

                    if (hasOffsetX) {
                        entity->setPositionX(entity->getPositionX() + intersection.size.width);
                    }

                    if (hasOffsetY) {
                        entity->setPositionY(entity->getPositionY() + intersection.size.height);
                    }
                }
            }
        }
    }
    


}
