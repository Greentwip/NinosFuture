#ifndef __WINDY_PLAYER_H__
#define __WINDY_PLAYER_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Sprite;
    class Ladder;
    class Browner;
    class VioletBrowner;
    class Bullet;
}


namespace windy {
    class Player : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void initVariables();

        virtual void walk();

        virtual void jump();

        virtual void dashJump();

        virtual void slide();

        virtual void climbShift();

        virtual void climb();

        virtual void move();

        virtual void triggerActions();

        virtual void onUpdate(float dt) override;

    public:
        bool atBattleEnd;
        bool atExit;
        bool demoMode;
        bool canMove;
        bool spawning;
        bool vulnerable;
        bool accomplishing;
        int maxFallSpeed;
        int bubbleTimer;
        int health;
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

    private:
        cocos2d::Point airSpeedBackup;
        bool cameraAirSpeedBackup;

        Sprite* sprite;

        friend class Browner;
        friend class VioletBrowner;
    };
}

#endif
