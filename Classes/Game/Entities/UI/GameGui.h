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

        void restoreHealth(int amount);
        void restoreWeapon(int amount);
        void restoreBossHealth(int amount);

        virtual void onUpdate(float dt) override;

    
        EnergyBar* healthBar;
        EnergyBar* weaponBar;
        EnergyBar* bossHealthBar;

    public:
        bool fillingHealthBar;
        bool fillingWeaponBar;
        bool fillingBossHealthBar;

    private:
        EnergyBar* fillTarget;

        float fillTimer;
        float fillTimeDelay;

        int fillAmount;

        bool filling;
    };
}

#endif
