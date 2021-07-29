#ifndef __WINDY_DIRECTIONAL_BULLET_H__
#define __WINDY_DIRECTIONAL_BULLET_H__

#include "cocos2d.h"

#include "Bullet.h"

namespace windy {
    class DirectionalBullet : public Bullet
    {
    public:
        static DirectionalBullet* create();

        virtual bool init() override;

        void setup();

        void fire(int power, int direction, GameTags::Weapon tag);

        void fire(int power, const cocos2d::Point& playerPosition, GameTags::Weapon tag);


        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void onFinished() override;

        virtual void onUpdate(float dt) override;
    };
}

#endif
