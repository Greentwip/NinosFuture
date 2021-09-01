#ifndef _WINDY_PHYSICS_WORLD_H_
#define _WINDY_PHYSICS_WORLD_H_

#include <vector>
#include <map>

#include "cocos2d.h"

#include "Entities/Logical.h"

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

        void setUnderwater(bool underwater);

        virtual void update(float dt);

        static std::map<CollisionContact, bool> getCollisionResult(Logical* entity, Logical* landscapeEntity);
        static void alignCollisions(Logical* entity, Logical* landscapeEntity, bool clearContacts = false);

        void unregisterContact(Logical* a, Logical* b);

        void resetContactEventCollisions();

        float gravity;

    private:
        Level* level;
        float maxFallSpeed;

        long long contactEventCollisionIndex;
        std::vector<std::pair<long long, std::pair<Logical*, Logical*>>> contactEventCollisions;

    };

}

#endif