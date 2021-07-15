#ifndef __WINDY_LOGICAL_H__
#define __WINDY_LOGICAL_H__

#include <vector>
#include "cocos2d.h"

#include "./../Level.h"

namespace windy {
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

        static cocos2d::Rect normalizeCollisionRectangle(cocos2d::Node*, cocos2d::Rect);

    public:
        Level* level;
        std::vector<cocos2d::Rect> collisionRectangles;

    protected:
        cocos2d::Point lastPosition;
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
