#ifndef __OPTIONS_SCENE_H__
#define __OPTIONS_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

namespace game {
    class Fader;
}

namespace game {
    class OptionsScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(OptionsScene);

    private:
        void save(float bgmVolume, float sfxVolume);

        cocos2d::Node* bgm;
        cocos2d::Node* sfx;
        cocos2d::Node* currentOption;

        cocos2d::ui::Slider* bgmSlider;
        cocos2d::ui::Slider* sfxSlider;

        float bgmVolume;
        float sfxVolume;

        bool triggered;
        bool _ready;

        Fader* _fader;


    };
}

#endif
