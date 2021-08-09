#ifndef _WINDY_PHYSICS_WORLD_H_
#define _WINDY_PHYSICS_WORLD_H_

#include <vector>
#include <map>

#include "cocos2d.h"

namespace windy {
    class Logical;
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

        static void alignCollisions(Logical* entity, Logical* landscapeEntity, bool clearContacts = false);

    private:
        Level* level;
        float gravity;
        float maxFallSpeed;

        long long contactEventCollisionIndex;
        std::vector<std::pair<long long, std::pair<Logical*, Logical*>>> contactEventCollisions;

    };

}

#endif