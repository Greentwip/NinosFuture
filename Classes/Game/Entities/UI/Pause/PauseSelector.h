#ifndef _GAME_PAUSE_SELECTOR_H_
#define _GAME_PAUSE_SELECTOR_H_

#include <string>
#include <vector>
#include <functional>

#include "cocos2d.h"

namespace windy {
    class Sprite;
}

namespace game {
    class PauseInterruptor;
    class PauseMenu;
}

namespace game {
    class PauseSelector : public cocos2d::Node
    {
    public:
        enum SelectorOrientation {
            OrientationDown,
            OrientationRight
        };

        enum SelectorDirection {
            DirectionUp,
            DirectionDown,
            DirectionLeft,
            DirectionRight
        };

    public:
        static void preloadResources();

        static PauseSelector* create(SelectorOrientation orientation);

        virtual bool init();

        PauseInterruptor* getSelectedItem();
        void setSelectedItem(PauseInterruptor* item);
        float calculateDistance(const cocos2d::Point& a, const cocos2d::Point& b);
        PauseInterruptor* findNearest(const std::vector<PauseInterruptor*>& items, SelectorDirection direction);
        void selectFromItems(const std::vector<PauseInterruptor*>& items);

    private:
        friend class PauseMenu;
        windy::Sprite* sprite;
        SelectorOrientation orientation;

        PauseInterruptor* selectedItem;
        
    };

}

#endif