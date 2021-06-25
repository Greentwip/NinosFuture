#ifndef _WINDY_LABEL_H_
#define _WINDY_LABEL_H_

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "cocos2d.h"

namespace windy {
    class LabelAction;
}

namespace windy {
    class Label : public cocos2d::Label
    {
    public:
        static Label* create(const std::string& text,
                             const std::string& filePath,
                             float size,
                             cocos2d::TextHAlignment hAlignment,
                             cocos2d::TextVAlignment vAlignment,
                             cocos2d::Point anchor = cocos2d::Point(0, 1),
                             std::shared_ptr<LabelAction> animation = nullptr);

        void startAnimation();

    private:
        void onNewStringCharacter();
        std::string text;
        int charCount;
        float delay;
        std::function<void()> onAnimationEnd;
    };

}

#endif