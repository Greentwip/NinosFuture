#ifndef __GAME_VERTICAL_DOOR_H__
#define __GAME_VERTICAL_DOOR_H__

#include <memory>

#include "cocos2d.h"

#include "Windy/Entities/Door.h"

namespace windy {
    class Sprite;
}

namespace game {
    class GameVerticalDoor : public windy::Door
    {
    public:
        virtual void setup() override;
        virtual void setupPrefix() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
    };
}

#endif
