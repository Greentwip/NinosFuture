#ifndef __WINDY_LOGICAL_H__
#define __WINDY_LOGICAL_H__

#include <map>
#include <vector>
#include <memory>

#include "cocos2d.h"

#include "./../Level.h"

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
        static Logical* create(Level* level, const cocos2d::Point& position, const cocos2d::Size& size);

        void setup(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) = 0;

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        virtual void onUpdate(float dt) = 0;

        static std::shared_ptr<cocos2d::Rect> normalizeCollisionRectangle(cocos2d::Node*, cocos2d::Rect);

    public:
        Level* level;

        std::shared_ptr<cocos2d::Rect> collisionBox;
        std::map<CollisionContact, bool> contacts;

    protected:
        cocos2d::Point lastPosition;
        std::vector<std::shared_ptr<cocos2d::Rect>> collisionRectangles;
    };
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


#endif
