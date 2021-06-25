#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace windy {
    class Label;
}

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

    windy::Label* startText;
    windy::Label* optionsText;
    windy::Label* selectedOption;
};
#endif
