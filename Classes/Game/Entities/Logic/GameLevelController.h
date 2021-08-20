#ifndef __GAME_LEVEL_CONTROLLER_H__
#define __GAME_LEVEL_CONTROLLER_H__

#include "cocos2d.h"

#include "Windy/Entities/LevelController.h"

namespace game {
    class PauseMenu;
    class GameGui;
    class GamePlayer;
    class Fader;
}

namespace game {
    class GameLevelController : public windy::LevelController
    {
    private:
        enum LevelState {
            Startup,
            Playing,
            Restarting,
            BossBattle,
            Succeeded,
            GameOver,
            Exit
        };

        LevelState levelState;

    public:
        virtual bool init() override;

        virtual void startup() override;

        virtual void restart(std::function<void()> onLevelRestarted) override;

        virtual void succeed() override;

        virtual void onUpdate(float dt) override;

    private:
        bool atVictory;
        float exitTimer;
        float exitTimeDelay;

        PauseMenu* pauseMenu;

        bool manualPause;

        bool cameraPause;

        GameGui* gui;

        GamePlayer* _player;

        Fader* _restartFader;
        Fader* _pauseFader;
    };
}

#endif
