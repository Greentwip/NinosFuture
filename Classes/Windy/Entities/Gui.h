#ifndef __WINDY_GUI_H__
#define __WINDY_GUI_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Gui : public Logical
    {
    public:
        virtual bool init() override;

        virtual void build() = 0;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void onUpdate(float dt) override;
    };
}

#endif
