#ifndef __GAME_TABAN_H__
#define __GAME_TABAN_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"
#include "Windy/Entities/Resources.h"

namespace game {
    class Taban : public GameEnemy
    {
        enum AttackState {
            Scanning,
            WakingUp,
            Attacking
        };

    public:
        static windy::Resources& getResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void setOrientation() override;

        virtual void attack(float dt) override;

    private:
        float moveSpeed;

        float wakeUpTimer;
        float wakeUpDelay;

        AttackState attackState;
    };
}

#endif
