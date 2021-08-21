#ifndef __GAME_ROLL_RUNNER_H__
#define __GAME_ROLL_RUNNER_H__

#include "cocos2d.h"
#include "Game/Entities/Enemies/GameEnemy.h"
#include "Game/Entities/Resources.h"

namespace game {

    class RollRunner: public GameEnemy
    {
    private:
        enum class Orientation {
            Left = -1,
            Right = 1
        };
    
    public:
        static game::Resources& getResources();
        void setup() override;
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
        void attack(float dt) override;

        virtual void setOrientation() override;

    private:
        void flip(Orientation _orientation);

    private:
        bool orientationSet;
        float walkSpeed;
        Orientation orientation;
    };

}

#endif
