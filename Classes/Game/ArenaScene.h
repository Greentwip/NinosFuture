#ifndef __GAME_ARENA_SCENE_H__
#define __GAME_ARENA_SCENE_H__

#include "cocos2d.h"

namespace game {
    class ArenaScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(ArenaScene);
    };
}

#endif
