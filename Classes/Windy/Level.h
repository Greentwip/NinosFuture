#ifndef __WINDY_LEVEL_H__
#define __WINDY_LEVEL_H__

#include <string>

#include "cocos2d.h"

#include "Windy/AudioManager.h"

namespace windy {
    class Door;
    class Bounds;
    class Camera;
    class Player;
}

namespace windy {
    class Level : public cocos2d::Node
    {
    public:
        static Level* create(const std::string& resourcesRootPath,
                             const std::string& tilemapRootPath,
                             const std::string& mug, 
                             windy::Sounds bgm);

        virtual bool init();

        bool paused();
        void pause(bool isPaused);

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

    public:
        

        Door* triggeringDoor;
        cocos2d::Vector<Door*> doors;

        Bounds* bounds;
        Camera* camera;

        Player* player;

    private:
        std::string resourcesRootPath;
        std::string tilemapRootPath;
        std::string mug;
        windy::Sounds bgm;

        bool isPaused;


    };
}

#endif
