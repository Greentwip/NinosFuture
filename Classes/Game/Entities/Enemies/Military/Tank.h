#ifndef __GAME_TANK_H__
#define __GAME_TANK_H__

#include "cocos2d.h"
#include "Game/Entities/Enemies/GameEnemy.h"
#include "Game/Entities/Resources.h"

namespace game {

    class Tank : public GameEnemy {
    private:
        enum class AttackState {
            Walking,
            Standing,
            Attacking,
        };

    public:
        static game::Resources& getResources();
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
        void setup() override;
        void fire();
        void attack(float dt) override;

    private:
        float _walkSpeed;
        AttackState _attackState;
    };

}


#endif
