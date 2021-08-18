#ifndef __GAME_PLAYER_H__
#define __GAME_PLAYER_H__

#include "cocos2d.h"

#include "Windy/Entities/Player.h"
#include "Windy/Entities/Browner.h"
#include "Game/Entities/Resources.h"

namespace windy {
    class Item;
}

namespace game {
    class GameGui;
}

namespace game {
    class GamePlayer : public windy::Player
    {
    public:
        static game::Resources& getResources();

        virtual bool init() override;

        virtual void onRestart() override;
        virtual void onSpawn() override;
        virtual void onPlayerExit() override;

        virtual void onItemAcquired(windy::Item* item) override;

        void switchBrowner(int brownerId);

        bool getAcquiringWeapon();
        void setAcquiringWeapon(bool acquiringWeapon);

        virtual void setupBrowners() override;

        virtual windy::Browner* getBrowner(int brownerId) override;


        virtual void kill(bool killAnimation) override;

        virtual void restoreHealth(int amount) override;

        virtual void restoreWeaponEnergy(int amount) override;

        virtual void checkHealth() override;

        virtual void onUpdate(float dt) override;


        GameGui* gui;


    private:
        cocos2d::Vector<windy::Browner*> browners;

        bool restoringHealth;
        bool restoringWeaponEnergy;

        bool _acquiringWeapon;

    };
}

#endif
