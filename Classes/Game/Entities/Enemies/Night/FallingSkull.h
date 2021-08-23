#ifndef __GAME_FALLING_SKULL_H__
#define __GAME_FALLING_SKULL_H__

#include "cocos2d.h"
#include "Game/Entities/Enemies/GameEnemy.h"
#include "Game/Entities/Resources.h"

namespace game {

    class FallingSkull : public GameEnemy {
    private:
        enum AttackState {
            Idle,
            Attacking
        };

    public:
        static game::Resources& getResources();
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
        void setup() override;

        void attack(float dt) override;

    private:
        AttackState _attackState;
    };

}


#endif
