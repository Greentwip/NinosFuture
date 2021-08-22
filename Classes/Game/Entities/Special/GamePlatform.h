#ifndef __GAME_PLATFORM_H__
#define __GAME_PLATFORM_H__

#include <memory>

#include "cocos2d.h"

#include "Windy/Entities/Block.h"
#include "Game/Entities/Resources.h"

namespace windy {
    class Sprite;
}

namespace game {
    class GamePlatform : public windy::Block
    {
    public:
        static game::Resources& getResources();

        virtual bool init() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void onCollisionEnter(windy::Logical* collision) override;
        virtual void onCollision(windy::Logical* collision) override;
        virtual void onCollisionExit(windy::Logical* collision) override;

        virtual void onUpdate(float dt) override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

    private:
        friend class windy::Logical;
        windy::Sprite* sprite;

        cocos2d::Point _centerPosition;

        int _radius;
        int _direction;

        bool _isPlayerStanding;
    };
}

#endif
