#ifndef __GAME_ITEM_H__
#define __GAME_ITEM_H__

#include <string>
#include <memory>

#include "cocos2d.h"

#include "Windy/Entities/Item.h"

namespace windy {
    class Sprite;
}

namespace game {
    class GameItem : public windy::Item
    {
    public:
        void setup(const std::string& content, bool forever);

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

    private:
        windy::Sprite* sprite;
    };
}

#endif
