#ifndef __WINDY_LOGICAL_H__
#define __WINDY_LOGICAL_H__

#include "cocos2d.h"

#include "./../Level.h"
#include "./../Armature.h"
#include "./../Sprite.h"
#include "Game/Entities/Resources.h"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace windy {
    class ObjectEntry;
}

namespace windy {

    enum CollisionContact {
        Up,
        Down,
        Left,
        Right
    };

    class Logical : public cocos2d::Node
    {
    public:


        template<typename T>
        static void composite(Logical* logical);
        template<typename T>
        static void composite(Logical* logical, const game::Resources& resources);

        static std::shared_ptr<cocos2d::Rect> buildEntryCollisionRectangle(const game::Resources& resources,
                                                                           const cocos2d::Point& position,
                                                                           const cocos2d::Size& size);

        template<typename T>
        static std::shared_ptr<cocos2d::Rect> buildEntryCollisionRectangle(const cocos2d::Point& position,
                                                                    const cocos2d::Size& size) {
            const auto& resources = T::getResources();
            return Logical::buildEntryCollisionRectangle(resources, position, size);
        }

        template<typename T>
        static Logical* create(Level* level, const cocos2d::Point& position, const cocos2d::Size& size, int updatePriority);


        void setup(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) = 0;

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        void setEntry(std::shared_ptr<ObjectEntry> entry);

        static std::shared_ptr<ObjectEntry> getEntry(std::shared_ptr<cocos2d::Rect> collisionRectangle, 
                                                     std::function<Logical*()> createFunction);

        template<typename T>
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        void finish();
        void finishForever();

        virtual void onFinished();

        virtual void update(float dt);

        virtual void recomputeCollisionRectangles();

        virtual void onCollisionEnter(Logical* collision);
        virtual void onCollision(Logical* collision);
        virtual void onCollisionExit(Logical* collision);

        virtual void onUpdate(float dt) = 0;

        static std::shared_ptr<cocos2d::Rect> normalizeCollisionRectangle(const cocos2d::Point& nodePosition, cocos2d::Rect rectangle);
        static std::shared_ptr<cocos2d::Rect> normalizeCollisionRectangle(cocos2d::Node*, cocos2d::Rect);

    public:
        Level* level;

        std::shared_ptr<cocos2d::Rect> collisionBox;
        std::map<CollisionContact, bool> contacts;

        cocos2d::Point speed;

        bool ignoreGravity;
        bool ignoreLandscapeCollision;
        bool ignoreEntityCollision;

        cocos2d::Point lastPosition;

    protected:
        cocos2d::Point lastCollisionPosition;
        std::vector<std::shared_ptr<cocos2d::Rect>> collisionRectangles;

    protected:
        std::shared_ptr<ObjectEntry> entry;

    private:
        int updatePriority;
    };
}

template<typename T>
void windy::Logical::composite(Logical* entity) {
    const game::Resources& resources = T::getResources();
    Logical::composite<T>(entity, resources);
}

template<typename T>
void windy::Logical::composite(Logical* logical, const game::Resources& resources) {
    auto* entity = dynamic_cast<T*>(logical);
    assert(entity);

    auto armature = Armature(resources._armaturePath);
    auto newAnchor = armature.get(resources._entityName).anchor;

    entity->sprite = Sprite::create(resources._spritePath, newAnchor);
    entity->addChild(entity->sprite);

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = entity->sprite->getContentSize();

    entity->collisionRectangles = armature.get(resources._entityName).collisionRectangles;

    auto collisionBoxCenter = cocos2d::Point(entity->collisionRectangles[0]->getMidX(), entity->collisionRectangles[0]->getMidY());

    for (int i = 0; i < entity->collisionRectangles.size(); ++i) {
        entity->collisionRectangles[i] = Logical::normalizeCollisionRectangle(entity, *entity->collisionRectangles[i]);
    }

    entity->collisionBox = entity->collisionRectangles[0];
    entity->sprite->setPosition(cocos2d::Point(entity->collisionBox->size.width * anchorChange.x, entity->collisionBox->size.height * anchorChange.y));
}


template<typename T>
windy::Logical* windy::Logical::create(windy::Level* level, const cocos2d::Point& position, const cocos2d::Size& size, int updatePriority) {
    Logical* logical = new (std::nothrow) T();

    if (logical) {
        logical->level = level;
        logical->updatePriority = updatePriority;
        logical->setup(position, size);
    }

    if (logical && logical->init()) {
        logical->autorelease();
        return logical;
    }

    CC_SAFE_DELETE(logical);
    return nullptr;

}

template<typename T>
std::shared_ptr<cocos2d::Rect> windy::Logical::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return T::getEntryCollisionRectangle(position, size);
}

#endif
