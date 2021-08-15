#ifndef __GAME_ENERGY_BAR_H__
#define __GAME_ENERGY_BAR_H__

#include "cocos2d.h"

#include "Windy/Entities/BarMeter.h"

namespace windy {
    class Sprite;
}

namespace game {
    class EnergyBar : public windy::BarMeter
    {
    public:
        static void preloadResources();

        virtual bool init() override;

        virtual void setValue(int value) override;

        virtual int getValue() override;

        int getMaxValue();

    public:
        windy::Sprite* sprite;

    private:
        cocos2d::Vector<windy::Sprite*> cells;

        int maxValue;
    };
}

#endif
