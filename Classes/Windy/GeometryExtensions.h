#ifndef WINDY_GEOMETRY_EXTENSIONS_H
#define WINDY_GEOMETRY_EXTENSIONS_H

#include <cmath>

#include "cocos2d.h"

namespace windy {
	class GeometryExtensions {
	public:
		static cocos2d::Rect rectIntersection(cocos2d::Rect rect1, cocos2d::Rect rect2) {
			auto intersection = 
				cocos2d::Rect(std::max(rect1.origin.x, rect2.origin.x),
							  std::max(rect1.origin.y, rect2.origin.y),
							  0,
							  0);

			intersection.size.width =
				std::min(rect1.origin.x + rect1.size.width, rect2.origin.x + rect2.size.width) - intersection.origin.x;

			intersection.size.height =
				std::min(rect1.origin.y + rect1.size.height, rect2.origin.y + rect2.size.height) - intersection.origin.y;

			return intersection;

		}
	};
}


#endif