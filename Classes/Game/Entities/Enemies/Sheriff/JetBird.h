#ifndef __GAME_JETBIRD_H__
#define __GAME_JETBIRD_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"

namespace game {
    class JetBird : public GameEnemy
    {
        enum AttackState {
            Scanning,
            Flying
        };

    public:
        static void preloadResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void setOrientation() override;

        virtual void attack(float dt) override;

    private:
        AttackState attackState;

        float flySpeed;
    };
}

#endif
