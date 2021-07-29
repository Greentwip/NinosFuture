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

        void fire(int power, int direction, GameTags::Weapon tag);

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const std::string& powerLevel,
                                                                         const cocos2d::Point& position, 
                                                                         const cocos2d::Size& size);

        virtual void onFinished() override;

        virtual void onUpdate(float dt) override;
    };
}

#endif
