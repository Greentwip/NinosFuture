#ifndef __STAGE_SELECT_SCENE_H__
#define __STAGE_SELECT_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class StageSelectScene : public cocos2d::Layer
{
public:
    virtual bool init();

    static cocos2d::Scene* scene();

    virtual void onEnter();
    virtual void onExit();

    virtual void update(float dt);

    CREATE_FUNC(StageSelectScene);
};
#endif
