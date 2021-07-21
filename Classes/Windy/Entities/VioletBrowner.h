#ifndef __WINDY_VIOLET_BROWNER_H__
#define __WINDY_VIOLET_BROWNER_H__

#include <string>

#include "cocos2d.h"

#include "Browner.h"

namespace windy {
    class VioletBrowner : public Browner
    {
    public:
        virtual void setBaseName() override;
        virtual void loadActions() override;
        virtual void spawn() override;
    };
}

#endif
