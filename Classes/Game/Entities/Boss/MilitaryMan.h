#ifndef __GAME_MILITARY_MAN_H__
#define __GAME_MILITARY_MAN_H__

#include "cocos2d.h"

#include "GameBoss.h"

namespace game {
    class MilitaryMan : public GameBoss
    {
    public:
        virtual void onSpawn() override;

        virtual void switchToBattleBrowner() override;

        virtual void onUpdate(float dt) override;

        void attack();

    private:
        enum AttackState {
            ShootingLeft1,
            DashingLeft1,
            ShootingLeft2,
            JumpingLeft,
            DashingLeft2,
            ShootingRight1,
            DashingRight1,
            ShootingRight2,
            JumpingRight,
            DashingRight2,
            CoolDown,
            Idle
        };

        int jumpCounter;
        int jumpAmount;

        AttackState attackState;


    };
}

#endif
