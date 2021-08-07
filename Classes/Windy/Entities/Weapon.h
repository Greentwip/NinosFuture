#ifndef __WINDY_WEAPON_H__
#define __WINDY_WEAPON_H__

#include "cocos2d.h"

#include "Logical.h"

#include "./../GameTags.h"

namespace windy {
    class Sprite;
}

namespace windy {

    // All sub weapons should include a setup method to create the sprite property, see the VioletBullet class for reference
    class Weapon : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

    public:
        int power;

        Sprite* sprite;
    };
}

#endif
