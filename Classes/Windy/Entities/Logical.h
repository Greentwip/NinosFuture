#ifndef __WINDY_LOGICAL_H__
#define __WINDY_LOGICAL_H__

#include <map>
#include <vector>
#include <memory>
#include <functional>

#include "cocos2d.h"

#include "./../Level.h"

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
        static void preloadResources();

        template<typename T>
        static Logical* create(Level* level, const cocos2d::Point& position, const cocos2d::Size& size);


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

        void recomputeCollisionRectangles();

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

    protected:
        cocos2d::Point lastPosition;
        std::vector<std::shared_ptr<cocos2d::Rect>> collisionRectangles;

    private:
        std::shared_ptr<ObjectEntry> entry;
    };
}

template<typename T>
void windy::Logical::preloadResources() {
    return T::preloadResources();
}

template<typename T>
windy::Logical* windy::Logical::create(windy::Level* level, const cocos2d::Point& position, const cocos2d::Size& size) {
    Logical* logical = new (std::nothrow) T();

    if (logical) {
        logical->level = level;
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
