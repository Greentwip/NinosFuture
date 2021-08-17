#ifndef __GAME_CANNONJOE_H__
#define __GAME_CANNONJOE_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"
#include "Game/Entities/Resources.h"

namespace game {
    class CannonJoe : public GameEnemy
    {
        enum AttackState {
            Before,
            Attacking,
            Cooldown,
            None
        };

    public:
        static game::Resources& getResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void setOrientation() override;

        virtual void attack(float dt) override;

    private:
        float attackTimer;
        float attackTimeInterval;

        AttackState attackState;

        int bulletPower;
    };
}

#endif
