#ifndef __WINDY_PASSAGE_H__
#define __WINDY_PASSAGE_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Passage : public Logical
    {
    public:
        virtual bool init() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void onUpdate(float dt) override;
    };
}

#endif
