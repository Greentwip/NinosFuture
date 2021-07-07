#ifndef __STAGE_SELECT_SCENE_H__
#define __STAGE_SELECT_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace windy {
    class Sprite;
}

class StageSelectScene : public cocos2d::LayerColor
{
public:
    virtual bool init();

    static cocos2d::Scene* scene();

    virtual void onEnter();
    virtual void onExit();

    virtual void update(float dt);

    CREATE_FUNC(StageSelectScene);

private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    cocos2d::Node* sheriffMan;
    cocos2d::Node* militaryMan;
    cocos2d::Node* vineMan;
    cocos2d::Node* nightMan;
    cocos2d::Node* swing;

    cocos2d::Node* selectedMug;

    windy::Sprite* cursor;

    std::string positionX;
    std::string positionY;

    bool triggered;
};
#endif
