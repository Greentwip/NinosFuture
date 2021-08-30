#ifndef __WINDY_SPIKES_H__
#define __WINDY_SPIKES_H__

#include "cocos2d.h"

#include "Enemy.h"

namespace windy {
    class Spikes : public Enemy
    {
    public:
        virtual void setup() override;

        virtual void onDefeated() override;

        virtual void setOrientation() override;

        virtual void attack(float dt) override;

        virtual void onCollisionEnter(Logical* collision) override;


        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void onUpdate(float dt) override;
    };
}

#endif
