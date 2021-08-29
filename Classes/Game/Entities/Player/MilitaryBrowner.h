#ifndef __GAME_MILITARY_BROWNER_H__
#define __GAME_MILITARY_BROWNER_H__

#include <string>

#include "cocos2d.h"

#include "Windy/Entities/Browner.h"

namespace game {
    class MilitaryBrowner : public windy::Browner
    {
    public:
        virtual void initConstraints() override;
        virtual void setBaseName() override;
        virtual void loadActions() override;
        virtual void spawn() override;
        virtual void attack() override;
        virtual void fire() override;
    };
}

#endif
