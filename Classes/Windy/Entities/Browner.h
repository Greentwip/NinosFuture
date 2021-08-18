#ifndef __WINDY_BROWNER_H__
#define __WINDY_BROWNER_H__

#include <string>

#include "cocos2d.h"

namespace windy {
    class Player;
    class Level;
}

namespace windy {
    class Browner : public cocos2d::Node
    {
    public:
        template<typename T>
        static Browner* create(Level* level, Player* player);

        virtual bool init();

        virtual void initConstraints();
        virtual void initVariables();

        virtual void setBaseName();

        virtual void loadActions() = 0;

        virtual void spawn();

        virtual void restoreWeaponEnergy(int amount);

        void activate();

        void deactivate();

        void runAction(const std::string& action);

        float getActionDuration(const std::string& action);

        int getCurrentAnimationNumberOfFrames();

        void stopActions();

        void stopAllActions();

        void pauseActions();

        void resumeActions();

        void increaseOrLoopFrame();

        int getSpriteNormal();

        void timedShoot();

        void charge();

        virtual void attack();

        virtual void fire() = 0;


    public:
        int brownerId;

        // Constraints
        bool canWalk;
        bool canJump;
        bool canDashJump;
        bool canSlide;
        bool canClimb;
        bool canAttack;
        bool canWalkShoot;
        bool canJumpShoot;
        bool canCharge;
        bool canMorph;
        bool hasIntro;
        bool canBeHurt;

        int energy;
        int maxEnergy;

        std::string chargePower;

        int chargeTimer;
        int attackTimer;

    protected:
        Level* level;
        Player* player;


        bool tintA;
        bool tintB;

        int chargeFxColor;

        std::string baseName;

        cocos2d::Color3B spriteColor;
    };
}

template<typename T>
windy::Browner* windy::Browner::create(windy::Level* level, windy::Player* player) {
    Browner* browner = new (std::nothrow) T();

    if (browner) {
        browner->level = level;
        browner->player = player;
    }

    if (browner && browner->init()) {
        browner->autorelease();
        browner->initConstraints();
        browner->initVariables();
        browner->setBaseName();
        browner->loadActions();
        browner->spawn();

        return browner;
    }

    CC_SAFE_DELETE(browner);
    return nullptr;

}

#endif
