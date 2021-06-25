#ifndef ANIMATION_ACTION_H
#define ANIMATION_ACTION_H

#include <string>

namespace windy {
	class AnimationAction {
	public:
		std::string name;
		std::string animationName;
		bool forever;
		float delay;

		AnimationAction(const std::string& name, const std::string animationName, bool forever, float delay) {
			this->name = name;
			this->animationName = animationName;
			this->forever = forever;
			this->delay = delay;

		}
	};
}

#endif
