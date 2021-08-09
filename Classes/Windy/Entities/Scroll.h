#ifndef __WINDY_SCROLL_H__
#define __WINDY_SCROLL_H__

#include "cocos2d.h"

#include "Logical.h"

#include "./../CameraFlags.h"

namespace windy {
    class Scroll : public Logical
    {
    public:
        virtual bool init() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;

        CameraFlags::CameraScroll scroll;
    };
}

#endif
