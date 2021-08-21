#ifndef __ABAKURA_SCENE_H__
#define __ABAKURA_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace windy {
    class Sprite;
}

namespace game {
    class AbakuraScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        void abakuraIntro();

        CREATE_FUNC(AbakuraScene);

    private:
        windy::Sprite* abakuraLogo;
    };
}

#endif
