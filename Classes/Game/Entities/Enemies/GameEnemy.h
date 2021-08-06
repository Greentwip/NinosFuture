#ifndef __GAME_ENEMY_H__
#define __GAME_ENEMY_H__

#include "cocos2d.h"

#include "Windy/Entities/Enemy.h"

namespace game {
    class GameEnemy : public windy::Enemy
    {

    public:
        virtual void onDefeated() override;

    };
}

#endif
