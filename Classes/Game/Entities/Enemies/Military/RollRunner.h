#ifndef __GAME_ROLL_RUNNER_H__
#define __GAME_ROLL_RUNNER_H__

#include "cocos2d.h"
#include "Game/Entities/Enemies/GameEnemy.h"

namespace game {

    class RollRunner: public GameEnemy
    {
        enum class Orientation {
            Left = -1,
            Right = 1
        };
    bool orientationSet = false;
    float walkSpeed = 1.0f;
    Orientation orientation = Orientation::Left;

    void flip(Orientation _orientation);

    public:
        static void preloadResources();
        void setup() override;
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
        void attack(float dt) override;
    };

}

#endif
