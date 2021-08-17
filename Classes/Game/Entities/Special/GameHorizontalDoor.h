#ifndef __GAME_HORIZONTAL_DOOR_H__
#define __GAME_HORIZONTAL_DOOR_H__

#include <memory>

#include "cocos2d.h"

#include "Windy/Entities/Door.h"
#include "Windy/Entities/Resources.h"

namespace windy {
    class Sprite;
}

namespace game {
    class GameHorizontalDoor : public windy::Door
    {
    public:
        static windy::Resources& getResources();

        virtual void setup() override;
        virtual void setupPrefix() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
    };
}

#endif
