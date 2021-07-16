#ifndef __WINDY_DEBUG_DRAW_NODE_H__
#define __WINDY_DEBUG_DRAW_NODE_H__

#include <cstdint>

#include "cocos2d.h"

#include "Entities/Logical.h"

namespace windy {
    class Level;
}

namespace windy {
    class DebugDrawNode : public cocos2d::Node
    {
    public:
        static DebugDrawNode* create(windy::Level* level);
        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual	void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);
        void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

        Level* level;

    private:
        cocos2d::CustomCommand renderCommand;

    };
}

#endif
