#ifndef _WINDY_SPRITE_H_
#define _WINDY_SPRITE_H_

#include <string>
#include <map>

#include "cocos2d.h"

#include "AnimationAction.h"

#include "SpriteDefinition.h"

namespace windy {
    class Sprite: public cocos2d::Sprite
    {
    public:
        static Sprite* create(const std::string& spriteName, const cocos2d::Point& anchor = cocos2d::Point(0.5f, 0.5f));
        void setImageIndex(int imageIndex);
        void increaseOrLoopFrame();
        Sprite* setAnimation(const std::string& animationName);
        cocos2d::Vector<cocos2d::SpriteFrame*>  getAnimationFrames(const std::string animationName);
        int getCurrentAnimationNumberOfFrames();
        int getImageIndex();
        std::string getSpriteFrameName();
        bool checkTouch(const cocos2d::Point& point);
        void appendAction(AnimationAction action, bool prependAction, std::string baseName = "");
        void appendActionSet(std::vector<AnimationAction> set, bool prependAction, std::string baseName = "");
        void loadDefinitions(const std::string& path);
        SpriteDefinition getDefinition(const std::string& name);
        cocos2d::ActionInterval* getCurrentAction();
        cocos2d::ActionInterval* getAction(const std::string& name, const std::string& baseName = "");
        void runAction(const std::string& name, const std::string& baseName = "");
        float getActionDuration(const std::string& name, const std::string& baseName = "");
        void reverseAction();
        void pauseActions();
        void resumeActions();
        void stopActions();

    private:
        std::string currentAnimation;
        int imageIndex;
        float touchRange;
        int cycles;
        std::vector<std::string> animations;
        std::map<std::string, std::map<int, std::string> > frames;
        cocos2d::Point defaultAnchor;
        cocos2d::Map<std::string, cocos2d::ActionInterval*> actions;

        cocos2d::ActionInterval* currentAction;

        std::map<std::string, SpriteDefinition> definitions;


    };

}

#endif