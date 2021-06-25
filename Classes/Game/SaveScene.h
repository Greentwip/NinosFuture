#ifndef __SAVE_SCENE_H__
#define __SAVE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SaveScene : public cocos2d::Layer
{
public:
    virtual bool init();

    static cocos2d::Scene* scene();

    virtual void onEnter();
    virtual void onExit();

    virtual void update(float dt);

    CREATE_FUNC(SaveScene);
};
#endif
