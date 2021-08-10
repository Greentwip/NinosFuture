#ifndef __GAME_COW_H__
#define __GAME_COW_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"

namespace game {
    class Cow : public GameEnemy
    {
        enum AttackState {
            Scanning,
            Morphing,
            Attacking,
            CoolDown
        };

    public:
        static void preloadResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void attack(float dt) override;

    private:
        float attackTimer;
        float attackTimeInterval;

        float morphTimer;
        float morphTimeInterval;

        AttackState attackState;

        int bulletPower;
    };
}

#endif
