#ifndef __GAME_ITEM_H__
#define __GAME_ITEM_H__

#include <string>
#include <memory>

#include "cocos2d.h"

#include "Windy/Entities/Item.h"
#include "Game/Entities/Resources.h"

namespace windy {
    class Sprite;
}

namespace game {
    class GameItem : public windy::Item
    {
    public:
        static game::Resources& getResources();

        virtual void setup(const std::string& name, const std::string& content, bool forever, bool collectible) override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void onFinished() override;

    private:
        bool permanent;
    };
}

#endif
