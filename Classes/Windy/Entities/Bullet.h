#ifndef __WINDY_BULLET_H__
#define __WINDY_BULLET_H__

#include "cocos2d.h"

#include "Logical.h"

#include "./../GameTags.h"

namespace windy {
    class Sprite;
}

namespace windy {

    // All sub weapons should include a setup method to create the sprite property, see the VioletBullet class for reference
    class Bullet : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void fire(int power, int direction, GameTags::Weapon tag);

    public:
        int power;

    protected:
        Sprite* sprite;
    };
}

#endif
