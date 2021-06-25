#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace windy {
    class Sprite;
}

namespace game {
    class IntroScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(IntroScene);

    private:
        void greentwipIntro();

        windy::Sprite* greentwipLogo;
    };
}


#endif
