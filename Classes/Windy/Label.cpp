#include "Label.h"

#include "LabelAction.h"

windy::Label* windy::Label::create(const std::string& text, 
                                   const std::string& filePath,
                                   float size, 
                                   cocos2d::TextHAlignment hAlignment,
                                   cocos2d::TextVAlignment vAlignment,
                                   cocos2d::Point anchor,
                                   std::shared_ptr<windy::LabelAction> animation) {

	Label* label = new (std::nothrow) Label();

	if (label && label->initWithTTF(text, filePath, size, cocos2d::Size(0, 0), hAlignment, vAlignment)) {
        label->autorelease();

        label->setAnchorPoint(anchor);

        if (animation != nullptr) {
            label->text = text;
            label->charCount = 0;
            label->delay = animation->delay;
            label->onAnimationEnd = animation->callback;
            label->setString("");
            label->getFontAtlas()->setAliasTexParameters();
            label->setVisible(false);
        }
        else {
        }

        return label;
    }

    CC_SAFE_DELETE(label);
    return nullptr;
}


void windy::Label::startAnimation() {
    this->setVisible(true);
    auto delay = cocos2d::DelayTime::create(this->delay);
    auto sequence = cocos2d::Sequence::create(delay, cocos2d::CallFunc::create([this]() { this->onNewStringCharacter(); }), nullptr);
    auto forever = cocos2d::RepeatForever::create(sequence);
    this->runAction(forever);
}

void windy::Label::onNewStringCharacter() {
    if (this->charCount < this->text.size()) {
        this->charCount = this->charCount + 1;

        this->setString(this->text.substr(0, this->charCount));
    }
    else {
        this->stopAllActions();
        if (this->onAnimationEnd) {
            this->onAnimationEnd();
        }
        
    }
}
