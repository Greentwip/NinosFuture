#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"

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
        void selectAbout();
        void selectProjects();
        void selectCredits();

        bool triggered;

        bool _ready;

        windy::Label* startText;
        windy::Label* optionsText;
        windy::Label* _aboutText;
        windy::Label* _projectsText;
        windy::Label* _creditsText;
        windy::Label* selectedOption;

        Fader* _fader;
    };
}

#endif
