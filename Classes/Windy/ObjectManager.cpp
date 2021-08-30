#include <algorithm>

#include "ObjectManager.h"

#include "Level.h"

#include "Entities/Logical.h"
#include "Entities/Bounds.h"

#include "Entities/ObjectEntry.h"

#include "GameTags.h"

#include "GeometryExtensions.h"

using namespace windy;

ObjectManager* ObjectManager::create(Level* level) {

    ObjectManager* objectManager = new (std::nothrow) ObjectManager();

    if (objectManager) {
        objectManager->level = level;
    }

    if (objectManager && objectManager->init()) {
        objectManager->autorelease();
        return objectManager;
    }

    CC_SAFE_DELETE(objectManager);
    return nullptr;
}

bool ObjectManager::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}


void ObjectManager::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, -2048, false);
}


void ObjectManager::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}


void ObjectManager::resetEntryTable(std::vector<std::shared_ptr<ObjectEntry>> newEntries) {

    for (int i = 0; i < newEntries.size(); ++i) {
        newEntries[i]->finished = false;
        newEntries[i]->finishedForever = false;
        newEntries[i]->respawnPrevention = false;
    }

    for (int i = 0; i < this->objectEntries.size(); ++i) {
        auto entry = this->objectEntries[i];

        entry->finished = false;
        entry->respawnPrevention = false;

        if (entry->mappedInstance != nullptr) {
            this->level->entities.eraseObject(entry->mappedInstance);

            entry->mappedInstance->onFinished();
            entry->mappedInstance->removeFromParent();
            entry->mappedInstance = nullptr;
        }
    }

    objectEntries = newEntries;

}

void ObjectManager::update(float dt)
{
    // Erase finished forever entities

    for (int i = 0; i < this->objectEntries.size(); ++i) {
        auto entry = this->objectEntries[i];

        if (entry->finishedForever) {
            if (entry->mappedInstance != nullptr) {
                entry->mappedInstance->onFinished();

                this->level->entities.eraseObject(entry->mappedInstance);
                entry->mappedInstance->removeFromParent();
                entry->mappedInstance = nullptr;
            }
        }
    }

    // Erase finished forever entries
    this->objectEntries.erase(std::remove_if(this->objectEntries.begin(), 
                                             this->objectEntries.end(), 
                                             [](const std::shared_ptr<ObjectEntry> entry) {
                                                 return entry->finishedForever;
                                             }), 
                              this->objectEntries.end());



    // We have to inflate the bounds by 96 pixels for intro and outro player position (offscreen location)

    auto boundsCollisionBox = this->level->bounds->collisionBox;
    //auto inflatedBoundsCollisionBox = *this->level->bounds->collisionBox;
    auto inflatedBoundsCollisionBox = this->level->bounds->inflate(cocos2d::Size(20, 20));

    for (int i = 0; i < this->objectEntries.size(); ++i) {
        auto entry = this->objectEntries[i];

        if (entry->finished && entry->mappedInstance != nullptr) {
            entry->mappedInstance->onFinished();

            this->level->entities.eraseObject(entry->mappedInstance);
            entry->mappedInstance->removeFromParent();
            entry->mappedInstance = nullptr;

        }


        auto entryCollisionBox = entry->collisionRectangle;

        if (GeometryExtensions::rectIntersectsRect(*boundsCollisionBox, *entryCollisionBox)) {
            if (entry->mappedInstance == nullptr && !entry->respawnPrevention) {
                entry->mappedInstance = entry->createFunction();

                entry->mappedInstance->setEntry(entry);

                this->level->addChild(entry->mappedInstance, entry->priority);
                this->level->entities.pushBack(entry->mappedInstance);
            }


            if (entry->mappedInstance != nullptr) {
                auto instanceCollisionBox = entry->mappedInstance->collisionBox;

                if (!GeometryExtensions::rectIntersectsRect(inflatedBoundsCollisionBox, *instanceCollisionBox)) {
                    entry->finished = true;

                    entry->mappedInstance->onFinished();

                    this->level->entities.eraseObject(entry->mappedInstance);
                    entry->mappedInstance->removeFromParent();

                    entry->mappedInstance = nullptr;
                }
                else {
                    entry->respawnPrevention = true;
                }
            }

        }
        else {

            if (entry->mappedInstance != nullptr) {
                auto instanceCollisionBox = entry->mappedInstance->collisionBox;

                if (!GeometryExtensions::rectIntersectsRect(inflatedBoundsCollisionBox, *instanceCollisionBox)) {
                    entry->finished = true;
                    entry->respawnPrevention = false;

                    entry->mappedInstance->onFinished();

                    this->level->entities.eraseObject(entry->mappedInstance);
                    entry->mappedInstance->removeFromParent();

                    entry->mappedInstance = nullptr;
                }
            }
            else {
                entry->respawnPrevention = false;
                entry->finished = false;

            }
           
        }

    }

    /*cocos2d::Vector<Logical*> collidingEntities;

    for (int i = 0; i < this->level->entities.size(); ++i) {
        auto entity = this->level->entities.at(i);

        if (entity->getTag() == GameTags::General::Player ||
            entity->getTag() == GameTags::General::Enemy ||
            entity->getTag() == GameTags::Weapon::WeaponPlayer ||
            entity->getTag() == GameTags::Weapon::WeaponEnemy) { // Kinematic characters first
            collidingEntities.pushBack(entity);
        }
    }

    // We have to inflate the bounds by 96 pixels for intro and outro player position (offscreen location)

    auto boundsCollisionBox = this->level->bounds->inflate(cocos2d::Size(96, 96));

    for (int i = 0; i < collidingEntities.size(); ++i) {
        auto entity = collidingEntities.at(i);

        auto entityCollisionBox = entity->collisionBox;


        if (!boundsCollisionBox.intersectsRect(*entityCollisionBox)) {
            this->level->entities.eraseObject(entity);
            entity->removeFromParent();
        }
    }*/

}
