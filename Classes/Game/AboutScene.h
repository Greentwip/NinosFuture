#ifndef __GAME_ABOUT_SCENE_H__
#define __GAME_ABOUT_SCENE_H__

#include "cocos2d.h"

namespace game {
    class Fader;
}

namespace game {
    class AboutScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(AboutScene);

    private:
        cocos2d::Node* _previousButton;
        cocos2d::Node* _nextButton;

        cocos2d::Node* _currentOption;

        cocos2d::Node* _root;

        int _menuIndex;

        bool _triggered;
        bool _ready;

        Fader* _fader;

    };
}

#endif
