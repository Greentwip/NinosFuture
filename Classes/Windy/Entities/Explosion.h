#ifndef __WINDY_EXPLOSION_H__
#define __WINDY_EXPLOSION_H__

#include <memory>

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Sprite;
}

namespace windy {
    class Explosion : public Logical
    {
    public:
        virtual bool init() override;

        virtual void setup(cocos2d::Color3B spriteColor) = 0;

        virtual void fire(float angle) = 0;

        virtual void onFinished() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void onUpdate(float dt) override;

    protected:
        friend class Logical;

        Sprite* sprite;
    };
}

#endif
