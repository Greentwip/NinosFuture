#ifndef __GAME_KNIFE_SOLDIER_H__
#define __GAME_KNIFE_SOLDIER_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"
#include "Game/Entities/Resources.h"

namespace game {
    class KnifeSoldier : public GameEnemy
    {
        enum AttackState {
            Idle,
            Attacking,
            Waiting
        };

    public:
        static game::Resources& getResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        void fire();

        virtual void attack(float dt) override;

    private:
        AttackState _attackState;

        int bulletPower;
    };
}

#endif
