#ifndef __WINDY_WATER_H__
#define __WINDY_WATER_H__

#include "cocos2d.h"

#include "Animation.h"

namespace windy {
    class Water : public Animation
    {
    public:
        virtual bool init() override;
    };
}

#endif
