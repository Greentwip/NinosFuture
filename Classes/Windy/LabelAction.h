#ifndef LABEL_ACTION_H
#define LABEL_ACTION_H

#include <functional>

namespace windy {
	class LabelAction {
	public:
		float delay;
		std::function<void()> callback;

		LabelAction(float delay, std::function<void()> callback) {
			this->delay = delay;
			this->callback = callback;
		}
	};
}

#endif
