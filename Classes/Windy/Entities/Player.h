#ifndef __WINDY_PLAYER_H__
#define __WINDY_PLAYER_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Sprite;
    class Ladder;
    class Browner;
}


namespace windy {
    class Player : public Logical
    {
    public:
        enum ScreenState {
            OnScreen,
            OffScreen
        };

    public:
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void setupBrowners() = 0;

        virtual void onPlayerExit();

        virtual void initVariables();

        virtual void onRestart();

        virtual void onSpawn();

        virtual void onCollisionEnter(Logical* collision) override;

        virtual void onCollision(Logical* collision) override;

        virtual void onCollisionExit(Logical* collision) override;

        virtual void stun(int power);

        virtual bool attackCondition(); 
        virtual bool chargeCondition(); 
        virtual bool dischargeCondition(); 

        virtual bool walkLeftCondition();

        virtual bool walkRightCondition();

        virtual bool jumpCondition();

        virtual bool stopJumpCondition();

        virtual bool dashJumpCondition();

        virtual bool slideCondition();

        virtual bool slideLeftCondition();

        virtual bool slideRightCondition();

        virtual bool climbUpCondition();

        virtual bool climbDownCondition();

        virtual void walk();

        virtual void jump();

        virtual void dashJump();

        virtual void slide();

        virtual void climbShift();

        virtual void climb();

        virtual void move();

        virtual void explode(float angleOffset);

        virtual void kill(bool killAnimation);

        virtual void restoreHealth(int amount);
        virtual void restoreWeaponEnergy(int amount);

        virtual void checkHealth();

        virtual void triggerActions();

        void cancelAttacks();

        void exit();

        virtual void onUpdate(float dt) override;



    public:
        bool atIntro;
        bool atExit;
        bool demoMode;
        bool canMove;
        bool spawning;
        bool vulnerable;
        bool accomplishing;
        int maxFallSpeed;
        int bubbleTimer;
        int health;
        int maxHealth;
        bool alive;
        bool holeDoor;
        bool inWater;
        Ladder* activeLadder;

        bool spawnFlag;

        // State variables
        bool walking;
        bool jumping;
        bool dashJumping;
        bool sliding;
        bool climbing;
        bool attacking;
        bool charging;
        bool stunned;
        bool flashing;
        bool onGround;

        int walkSpeed;
        int climbSpeed;
        int slideSpeed;
        float jumpSpeed;
        float dashJumpSpeed;
        int stunTimer;
        int slideTimer;
        int climbCounter;

        bool largeSlide;

        Browner* currentBrowner;

        Sprite* sprite;

        ScreenState screenState;

    private:
        cocos2d::Point shiftSpeedBackup;
        bool cameraShiftSpeedBackup;

        float teleportTransitionSpeed;

        friend class Browner;
    };
}

#endif
