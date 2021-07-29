#ifndef __WINDY_ITEM_H__
#define __WINDY_ITEM_H__

#include <string>

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Sprite;
}

namespace windy {
    class Item : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        void setup(const std::string& content, bool forever);
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void onUpdate(float dt) override;

        int id;

    private:
        Sprite* sprite;
    };
}

#endif
