#ifndef __GAME_SPLASH_H__
#define __GAME_SPLASH_H__

#include <memory>

#include "cocos2d.h"

#include "Game/Entities/Resources.h"

namespace windy {
    class Sprite;
}

namespace game {
    class GameSplash : public cocos2d::Node
    {
    public:
        static game::Resources& getResources();

        static GameSplash* create();

        virtual bool init() override;

        void triggerSplash();

    public:
        windy::Sprite* sprite;
    };
}

#endif
