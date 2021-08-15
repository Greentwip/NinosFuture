#ifndef __WINDY_ENEMY_H__
#define __WINDY_ENEMY_H__

#include "cocos2d.h"

#include "Logical.h"

#include <random>

namespace windy {
    class Sprite;
    class Level;
}

namespace windy {
    class Enemy : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void setup() = 0;

        virtual void setOrientation();

        int getSpriteNormal();

        void checkHealth();

        virtual void onDefeated() = 0;

        virtual void attack(float dt) = 0;

        virtual void onCollisionEnter(Logical* collision) override;

        virtual void onUpdate(float dt) override;


        int power;
        int health;
        int maxHealth;

    protected:
        friend class Logical;
        friend class Level;

        Sprite* sprite;

        static std::random_device itemRandomDevice;
        static std::default_random_engine itemRandomEngine;

    };
}

#endif
