#ifndef __GAME_TREMOR_H__
#define __GAME_TREMOR_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"

namespace game {
    class TremorTail;
}

namespace game {
    class Tremor : public GameEnemy
    {
        enum AttackState {
            Moving,
            Attacking,
            Working,
            None
        };

    public:
        static void preloadResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void setOrientation() override;

        virtual void attack() override;

        virtual void onFinished() override;

        virtual void onDefeated() override;


        AttackState attackState;

    private:
        void onCannonAttackEnd();

        cocos2d::Point startPosition;

        TremorTail* tail;
    };
}

#endif
