#ifndef __GAME_RIP_H__
#define __GAME_RIP_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"
#include "Game/Entities/Resources.h"

namespace game {
    class Rip : public GameEnemy
    {
        enum AttackState {
            Jumping,
            Attacking,
            None
        };

    public:
        static game::Resources& getResources();

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
