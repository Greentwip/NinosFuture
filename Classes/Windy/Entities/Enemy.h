#ifndef __WINDY_ENEMY_H__
#define __WINDY_ENEMY_H__

#include "cocos2d.h"

#include "Logical.h"

#include <random>

namespace windy {
    class Sprite;
}

namespace windy {
    class Enemy : public Logical
    {
        enum AttackState {
            Before,
            Attacking,
            Cooldown,
            None
        };

    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);


        virtual void setOrientation();

        int getSpriteNormal();

        void checkHealth();

        virtual void onDefeated();

        virtual void attack();

        virtual void onCollisionEnter(Logical* collision) override;

        virtual void onUpdate(float dt) override;


        int power;
        int health;
        int maxHealth;

        bool isFlipping;
        bool attacking;

        int attackTimer;
        int attackTimeInterval;

        AttackState attackState;

    private:

        int bulletPower;

        Sprite* sprite;

        static std::random_device itemRandomDevice;
        static std::default_random_engine itemRandomEngine;
    };
}

#endif
