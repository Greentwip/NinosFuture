#ifndef __GAME_GAME_OVER_SCENE_H__
#define __GAME_GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <string>

namespace game {
    class Fader;
}

namespace game {
    class GameOverScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(GameOverScene);

    private:
        cocos2d::Node* _yesButton;
        cocos2d::Node* _noButton;

        cocos2d::Node* _currentOption;

        bool _triggered;
        bool _ready;

        Fader* _fader;

    };
}

#endif
