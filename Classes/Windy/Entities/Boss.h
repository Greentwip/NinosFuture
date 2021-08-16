#ifndef __WINDY_BOSS_H__
#define __WINDY_BOSS_H__

#include "cocos2d.h"

#include "Player.h"

namespace windy {
    class Browner;
}

namespace windy {
    class Boss : public windy::Player
    {
    public:
        enum BossState {
            Teleporting,
            Intro,
            Spawning,
            RestoringHealth,
            Fighting,
            Defeated
        };

    public:
        virtual bool init() override;

        virtual bool attackCondition() override;

        virtual bool chargeCondition() override;

        virtual bool dischargeCondition() override;

        virtual bool walkLeftCondition() override;

        virtual bool walkRightCondition() override;

        virtual bool jumpCondition() override;

        virtual bool stopJumpCondition() override;

        virtual bool dashJumpCondition() override;

        virtual bool slideCondition() override;

        virtual bool slideLeftCondition() override;

        virtual bool slideRightCondition() override;

        virtual bool climbUpCondition() override;

        virtual bool climbDownCondition() override;


        virtual void onCollisionEnter(Logical* collision) override;

        virtual void onCollision(Logical* collision) override;

        virtual void onCollisionExit(Logical* collision) override;

        virtual void checkHealth() override;

        virtual void switchToBattleBrowner() = 0;

        virtual void onUpdate(float dt) override;


        int power;

        BossState state;


    protected:

        bool triggerAttack;
        bool triggerCharge;
        bool triggerDischarge;

        bool triggerWalkLeft;
        bool triggerWalkRight;
        
        bool triggerJump;
        bool triggerStopJump;
        bool triggerDashJump;

        bool triggerSlide;
        bool triggerSlideLeft;
        bool triggerSlideRight;

        bool triggerClimbUp;
        bool triggerClimbDown;

    private:
        float introTimer;
    };
}

#endif
