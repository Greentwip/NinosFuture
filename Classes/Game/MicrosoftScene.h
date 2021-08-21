#ifndef __GAME_MICROSOFT_SCENE_H__
#define __GAME_MICROSOFT_SCENE_H__

#include "cocos2d.h"

namespace game {
    class MicrosoftScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        void intro();

        CREATE_FUNC(MicrosoftScene);
    };
}

#endif
