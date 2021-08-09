#ifndef __GAME_TELEPORT_BROWNER_H__
#define __GAME_TELEPORT_BROWNER_H__

#include <string>

#include "cocos2d.h"

#include "Windy/Entities/Browner.h"

namespace game {
    class TeleportBrowner : public windy::Browner
    {
    public:
        virtual void setBaseName() override;
        virtual void loadActions() override;
        virtual void spawn() override;
        virtual void fire() override;
    };
}

#endif
