#ifndef __GAME_VINE_MAN_H__
#define __GAME_VINE_MAN_H__

#include "cocos2d.h"

#include "GameBoss.h"

namespace game {
    class VineMan : public GameBoss
    {
    public:
        virtual void onSpawn() override;

        virtual void switchToBattleBrowner() override;

        virtual void onUpdate(float dt) override;

        void fire(const cocos2d::Point& weaponSpeed = cocos2d::Point(1.2f, 4.0f));

        void attack();

    private:
        enum AttackState {
            ShootingLeft1,
            FullDashLeft,
            DashingLeft1,
            ShootingLeft2,
            JumpingLeft,
            DashingLeft2,
            ShootingRight1,
            FullDashRight,
            DashingRight1,
            ShootingRight2,
            JumpingRight,
            DashingRight2,
            CoolDown,
            Idle
        };

        int jumpCounter;
        int jumpAmount;

        int attackCounter = 0;

        bool _isFullDash;

        AttackState attackState;


    };
}

#endif
