#ifndef __WINDY_CAMERA_SCROLL_H__
#define __WINDY_CAMERA_SCROLL_H__

#include "cocos2d.h"

#include "Logical.h"

#include "./../CameraFlags.h"

namespace windy {
    class CameraScroll : public Logical
    {
    public:
        virtual bool init() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;

        enum CameraFlags::CameraScroll scroll;
    };
}

#endif
