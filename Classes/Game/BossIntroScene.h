#ifndef __BOSS_INTRO_SCENE_H__
#define __BOSS_INTRO_SCENE_H__

#include "cocos2d.h"

namespace windy {
    class Sprite;
    class Label;    
}

namespace game {
    class Intro;
}

namespace game {
    class BossIntroScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(BossIntroScene);

    private:
        void onIntroComplete();
        void onBossIntroComplete();

        bool triggered;

        int bgmId;
        windy::Sprite* boss;
        windy::Label* text;

        Intro* intro;
    };
}


#endif
