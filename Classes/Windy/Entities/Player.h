#ifndef __WINDY_PLAYER_H__
#define __WINDY_PLAYER_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Sprite;
}


namespace windy {
    class Player : public Logical
    {
    public:
        virtual bool init() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;

    public:
        cocos2d::Rect collisionRectangle;

        bool alive;

    private:
        Sprite* sprite;
    };
}

#endif
