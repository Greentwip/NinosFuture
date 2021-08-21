#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"

namespace windy {
    class Sprite;
}

namespace game {
    class IntroScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        CREATE_FUNC(IntroScene);

    private:
        void greentwipIntro();

        windy::Sprite* greentwipLogo;
    };
}


#endif
