#ifndef __GAME_LYRIC_H__
#define __GAME_LYRIC_H__

#include "cocos2d.h"

#include "Game/Entities/Enemies/GameEnemy.h"
#include "Game/Entities/Resources.h"

namespace game {
    class Lyric : public GameEnemy
    {
        enum AttackState {
            Attacking
        };

    public:
        static game::Resources& getResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void setOrientation() override;

        virtual void attack(float dt) override;

    private:
        float moveSpeed;

        AttackState attackState;
    };
}

#endif
