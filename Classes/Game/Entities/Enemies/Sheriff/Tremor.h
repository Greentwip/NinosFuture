#ifndef __GAME_TREMOR_H__
#define __GAME_TREMOR_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"
#include "Windy/Entities/Resources.h"

namespace game {
    class TremorTail;
}

namespace windy {
    class ObjectEntry;
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
        static windy::Resources& getResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void setOrientation() override;

        virtual void attack(float dt) override;

        virtual void onFinished() override;

        virtual void onDefeated() override;


        AttackState attackState;

    private:
        void onCannonAttackEnd();

        cocos2d::Point startPosition;

        TremorTail* tail;

        std::shared_ptr<windy::ObjectEntry> _laserEntry;
    };
}

#endif
