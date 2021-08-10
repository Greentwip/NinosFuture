#ifndef __GAME_BARREL_H__
#define __GAME_BARREL_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"

namespace game {
    class Barrel : public GameEnemy
    {
        enum AttackState {
            Scanning,
            Standing,
            Walking,
            Sitting,
            CoolDown
        };

    public:
        static void preloadResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void attack(float dt) override;

    private:
        AttackState attackState;

        bool isStandReversed;

        float walkSpeed;

        float standTimer;
        float standTimeInterval;

        float walkTimer;
        float walkTimeInterval;

        float cooldownTimer;
        float cooldownTimeInterval;

    };
}

#endif
