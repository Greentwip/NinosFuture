#ifndef __GAME_LEVEL_CONTROLLER_H__
#define __GAME_LEVEL_CONTROLLER_H__

#include "cocos2d.h"

#include "Windy/Entities/LevelController.h"

namespace game {
    class GameLevelController : public windy::LevelController
    {
    public:
        virtual void restart() override;
    };
}

#endif
