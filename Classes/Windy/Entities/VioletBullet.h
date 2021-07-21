#ifndef __WINDY_VIOLET_BULLET_H__
#define __WINDY_VIOLET_BULLET_H__

#include <string>

#include "cocos2d.h"

#include "Bullet.h"

namespace windy {
    class VioletBullet : public Bullet
    {
    public:
        static VioletBullet* create();

        virtual bool init() override;

        void setup(const std::string& powerLevel);

        virtual void onUpdate(float dt) override;
    };
}

#endif
