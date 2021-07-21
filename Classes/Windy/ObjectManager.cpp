#include "ObjectManager.h"

#include "Level.h"

#include "Entities/Logical.h"
#include "Entities/Bounds.h"

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
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void ObjectManager::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void ObjectManager::update(float dt)
{

    cocos2d::Vector<Logical*> collidingEntities;

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
    }

}
