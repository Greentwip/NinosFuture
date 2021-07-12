#ifndef __WINDY_LEVEL_H__
#define __WINDY_LEVEL_H__

#include <string>

#include "cocos2d.h"

#include "Windy/AudioManager.h"

namespace windy {
    class Level : public cocos2d::Node
    {
    public:
        static Level* create(const std::string& mapPath, const std::string& mug, windy::Sounds bgm);

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);


    private:
        std::string mapPath;
        std::string mug;
        windy::Sounds bgm;


    };
}

#endif
