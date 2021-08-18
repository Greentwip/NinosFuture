#ifndef _GAME_PAUSE_INTERRUPTOR_H_
#define _GAME_PAUSE_INTERRUPTOR_H_

#include "cocos2d.h"

#include "Game/Entities/Resources.h"

#include <functional>
#include <string>

namespace windy {
    class Sprite;
}

namespace game {
    class PauseSelector;
    class PauseMenu;
    class EnergyBar;
    class GameLevelController;
}

namespace game {
    class PauseInterruptor : public cocos2d::Node
    {
    public:
        enum BrownerIdentifier {
            Invalid = -2
        };

    public:
        static game::Resources& getResources();

        static PauseInterruptor* create(const std::string& animation, std::function<void(PauseInterruptor* sender)> onTriggered);
        
        virtual bool init();

        void setVisitable(bool visitable);
        void visitInterruptor();
        void leaveInterruptor();
        void triggerInterruptor();
        void setTriggerCallback(std::function<void(PauseInterruptor* sender)> onTriggered);

    private:
        friend class PauseSelector;
        friend class PauseMenu;
        friend class GameLevelController;

        windy::Sprite* sprite;
        std::string animation;
        bool visitable;
        std::function<void(PauseInterruptor* sender)> onTriggered;

        int brownerId;
        std::string pauseItem;
        EnergyBar* energyBar;

    };

}

#endif
