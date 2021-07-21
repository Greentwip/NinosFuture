#ifndef _WINDY_OBJECT_MANAGER_H_
#define _WINDY_OBJECT_MANAGER_H_

#include "cocos2d.h"

namespace windy {
    class Level;
}


namespace windy {
    class ObjectManager : public cocos2d::Node
    {
    public:
        static ObjectManager* create(Level* level);

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

    private:
        Level* level;
    };

}

#endif