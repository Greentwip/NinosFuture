#ifndef _WINDY_PHYSICS_WORLD_H_
#define _WINDY_PHYSICS_WORLD_H_

#include "cocos2d.h"

namespace windy {
    class Level;
}


namespace windy {
    class PhysicsWorld : public cocos2d::Node
    {
    public:
        static PhysicsWorld* create(Level* level);

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

    private:
        Level* level;

    };

}

#endif