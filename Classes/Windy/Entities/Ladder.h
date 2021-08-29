#ifndef __WINDY_LADDER_H__
#define __WINDY_LADDER_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Block;
}

namespace windy {
    class Ladder : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        void solidify();
        void unsolidify();

        virtual void onUpdate(float dt) override;

    public:
        bool solidified;

    public:
        Block* ceiling;
    };
}

#endif
