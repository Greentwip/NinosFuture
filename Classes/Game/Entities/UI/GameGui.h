#ifndef __GAME_GUI_H__
#define __GAME_GUI_H__

#include "cocos2d.h"

#include "Windy/Entities/Gui.h"

namespace game {
    class EnergyBar;
}

namespace game {
    class GameGui : public windy::Gui
    {
    public:
        virtual void build() override;

        EnergyBar* healthBar;
        EnergyBar* weaponBar;
    };
}

#endif
