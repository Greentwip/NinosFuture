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
    class Logical;
    class DebugDrawNode;
    class PhysicsWorld;
    class ObjectManager;
    class LandscapeTile;
    class Gui;
    class LevelController;
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

        void restart();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

    public:
        

        Door* triggeringDoor;
        cocos2d::Vector<Door*> verticalDoors;
        cocos2d::Vector<Door*> horizontalDoors;

        Bounds* bounds;
        Camera* camera;

        Player* player;

        cocos2d::Vector<Logical*> entities;

        PhysicsWorld* physicsWorld;
        ObjectManager* objectManager;

        Gui* gui;

    private:
        DebugDrawNode* debugDrawNode;

        LevelController* levelController;

        std::string resourcesRootPath;
        std::string tilemapRootPath;
        std::string mug;
        windy::Sounds bgm;

        bool isPaused;

        Logical* lastCheckpoint;

        cocos2d::Vector<LandscapeTile*> tiles;
        cocos2d::Vector<LandscapeTile*> screenTiles;

    };
}

#endif
