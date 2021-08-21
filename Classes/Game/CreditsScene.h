#ifndef __GAME_CREDITS_SCENE_H__
#define __GAME_CREDITS_SCENE_H__

#include "cocos2d.h"

namespace game {
    class Fader;
}

namespace game {
    class CreditsScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(CreditsScene);

    private:
        bool _triggered;
        bool _ready;

        Fader* _fader;

    };
}

#endif
