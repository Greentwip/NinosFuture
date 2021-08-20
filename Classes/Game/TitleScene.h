#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace windy {
    class Label;
}

namespace game {
    class Fader;
}

namespace game {
    class TitleScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(TitleScene);

    private:
        void selectStart();
        void selectOptions();

        bool triggered;

        bool _ready;

        windy::Label* startText;
        windy::Label* optionsText;
        windy::Label* selectedOption;

        Fader* _fader;
    };
}

#endif
