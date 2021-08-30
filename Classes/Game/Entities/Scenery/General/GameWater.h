#ifndef __GAME_WATER_H__
#define __GAME_WATER_H__

#include "cocos2d.h"

#include "Windy/Entities/Water.h"

namespace game {
    class GameSplash;
}

namespace game {
    class GameWater : public windy::Water
    {
    public:
        static game::Resources& getResources();

        virtual void setup() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void onCollisionEnter(windy::Logical* collision) override;
        virtual void onCollisionExit(windy::Logical* collision) override;

    private:
        GameSplash* splash;
    };
}

#endif
