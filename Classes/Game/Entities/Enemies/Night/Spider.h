#ifndef __GAME_SPIDER_H__
#define __GAME_SPIDER_H__

#include "cocos2d.h"
#include "Game/Entities/Enemies/GameEnemy.h"
#include "Game/Entities/Resources.h"

namespace game {

    class Spider : public GameEnemy {
    public:
        static game::Resources& getResources();
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
        void setup() override;

        void attack(float dt) override;
    };

}


#endif
