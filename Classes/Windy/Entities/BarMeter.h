#ifndef __WINDY_BAR_METER_H__
#define __WINDY_BAR_METER_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class BarMeter : public Logical
    {
    public:
        virtual bool init() override;

        virtual void setValue(int value) = 0;
        
        virtual int getValue() = 0;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void onUpdate(float dt) override;

    protected:
        int meter;
    };
}

#endif
