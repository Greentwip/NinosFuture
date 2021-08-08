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
    public:
        static void composite(Enemy* enemy, const std::string& armaturePath, const std::string& spritePath, const std::string& enemyName);
        static std::shared_ptr<cocos2d::Rect> buildEntryCollisionRectangle(const cocos2d::Point& position, 
                                                                           const cocos2d::Size& size, 
                                                                           const std::string& armaturePath, 
                                                                           const std::string& enemyName);

        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void setup() = 0;

        virtual void setOrientation();

        int getSpriteNormal();

        void checkHealth();

        virtual void onDefeated() = 0;

        virtual void attack() = 0;

        virtual void onCollisionEnter(Logical* collision) override;

        virtual void onUpdate(float dt) override;


        int power;
        int health;
        int maxHealth;

    protected:
        Sprite* sprite;

        static std::random_device itemRandomDevice;
        static std::default_random_engine itemRandomEngine;
    };
}

#endif
