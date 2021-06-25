#include "Label.h"

windy::Label* windy::Label::create() {

	Label* label = new (std::nothrow) Label();
	if (label && label->init()) {
        label->autorelease();

        return label;
    }

    CC_SAFE_DELETE(label);
    return nullptr;
}
