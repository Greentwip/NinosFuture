#ifndef _GAME_PAUSE_MENU_H_
#define _GAME_PAUSE_MENU_H_

#include <string>

#include "cocos2d.h"

namespace windy {
    class Sprite;
    class Label;
    class Player;
}

namespace game {
    class PauseSelector;
    class PauseInterruptor;
    class PauseAnimation;
    class GameGui;
    class GameLevelController;
}

namespace game {
    class PauseMenu : public cocos2d::Node
    {
    public:
        static void preloadResources();

        static PauseMenu* create(windy::Player* player, GameGui* gui);

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);


    private:
        void setupBrowners();
        void initCallbacks();
        void prepare();
        void validateWeapons();

        PauseInterruptor* getBrowner(int brownerId);
        void setDefaultBrowner();
        
    public:
        PauseInterruptor* selectedBrowner;

    private:
        friend class GameLevelController;

        windy::Player* player;

        GameGui* gui;

        windy::Sprite* background;
        windy::Sprite* box;

        PauseSelector* selector;

        PauseInterruptor* eTank;
        PauseInterruptor* mTank;

        windy::Label* livesLabel;
        windy::Label* eTanksLabel;
        windy::Label* mTanksLabel;

        PauseInterruptor* exSwitch;
        PauseInterruptor* helmetSwitch;
        PauseInterruptor* exitSwitch;

        PauseInterruptor* health;

        PauseAnimation* weaponAnimation;


        std::vector<PauseInterruptor*> menuItems;
        std::vector<PauseInterruptor*> menuBrowners;
        std::vector<PauseInterruptor*> allMenuItems;

        PauseInterruptor* defaultBrowner;

        bool busy;
        bool fillingMTank;
        std::function<void(PauseInterruptor* sender)> onETankTriggered;
        std::function<void(PauseInterruptor* sender)> onMTankTriggered;
        std::function<void(PauseInterruptor* sender)> onBrownerTriggered;
        std::function<void(PauseInterruptor* sender)> onSwitchTriggered;


    };

}

#endif
