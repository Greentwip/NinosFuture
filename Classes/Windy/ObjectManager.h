#ifndef _WINDY_OBJECT_MANAGER_H_
#define _WINDY_OBJECT_MANAGER_H_

#include <vector>
#include <memory>

#include "cocos2d.h"

namespace windy {
    class Level;
    class ObjectEntry;
}


namespace windy {
    class ObjectManager : public cocos2d::Node
    {
    public:
        static ObjectManager* create(Level* level);

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        void resetEntryTable(std::vector<std::shared_ptr<ObjectEntry>> newEntries);

        virtual void update(float dt);

        std::vector<std::shared_ptr<ObjectEntry>> objectEntries;

    private:
        Level* level;
    };

}

#endif