#ifndef __GAME_PLAYER_BULLET_H__
#define __GAME_PLAYER_BULLET_H__

#include "cocos2d.h"

#include "Windy/Entities/Player.h"

namespace game {
    class GamePlayer : public windy::Player
    {
    public:
        static void preloadResources();

        virtual bool init() override;

        virtual void setupBrowners() override;
    };
}

#endif
