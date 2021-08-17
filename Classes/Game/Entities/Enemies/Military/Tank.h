#ifndef __GAME_TANK_H__
#define __GAME_TANK_H__

#include "cocos2d.h"
#include "Game/Entities/Enemies/GameEnemy.h"
#include "Windy/Entities/Resources.h"

namespace game {

    class Tank : public GameEnemy
    {
        float _walkSpeed = 35.0f;
        bool _attacking = false;
        bool _moving = false;
    public:
        static windy::Resources& getResources();
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
        void setup() override;
        void attack(float dt) override;
    };

}


#endif
