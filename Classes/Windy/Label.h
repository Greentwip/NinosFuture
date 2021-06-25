#ifndef _WINDY_LABEL_H_
#define _WINDY_LABEL_H_

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "cocos2d.h"

namespace windy {
    class Label : public cocos2d::Label
    {
    public:
        static Label* create();
    };

}

#endif