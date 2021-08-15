#ifndef __WINDY_ITEM_H__
#define __WINDY_ITEM_H__

#include <string>

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Sprite;
    class Level;
}

namespace windy {
    class Item : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;

        virtual void setup(const std::string& name, const std::string& content, bool forever, bool collectible) = 0;

        int id;

        std::string name;

        bool collectible;

    protected:
        friend class Level;
        friend class Logical;

        Sprite* sprite;
    };
}

#endif
