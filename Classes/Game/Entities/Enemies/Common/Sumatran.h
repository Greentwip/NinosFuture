#ifndef __GAME_SUMATRAN_H__
#define __GAME_SUMATRAN_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"
#include "Windy/Entities/Resources.h"

namespace game {
    class Sumatran : public GameEnemy
    {
        enum AttackState {
            Jumping,
            Attacking,
            None
        };

    public:
        static windy::Resources& getResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void attack(float dt) override;

    private:

        float attackTimer;
        float attackTimeInterval;

        AttackState attackState;

        cocos2d::Point jumpSpeed;
    };
}

#endif
