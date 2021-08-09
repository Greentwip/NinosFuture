#ifndef __WINDY_ANIMATION_H__
#define __WINDY_ANIMATION_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Sprite;
}

namespace windy {

    // All sub weapons should include a setup method to create the sprite property, see the VioletBullet class for reference
    class Animation : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

    public:
        Sprite* sprite;
    };
}

#endif
