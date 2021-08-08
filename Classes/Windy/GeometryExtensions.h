#ifndef WINDY_GEOMETRY_EXTENSIONS_H
#define WINDY_GEOMETRY_EXTENSIONS_H

#include <cmath>

#include "cocos2d.h"

namespace windy {
	class GeometryExtensions {
	public:
		static cocos2d::Rect rectIntersection(const cocos2d::Rect& rect1, const cocos2d::Rect& rect2) {
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

		static bool rectIntersectsRect(const cocos2d::Rect& rect1, const cocos2d::Rect& rect2) {
			return !(rect1.getMaxX()  < rect2.getMinX() ||
					 rect2.getMaxX()  < rect1.getMinX() ||
					 rect1.getMaxY()  < rect2.getMinY() ||
					 rect2.getMaxY()  < rect1.getMinY());

		}

		static cocos2d::Point moveTowards(const cocos2d::Point& current, const cocos2d::Point& target, float maxDistanceDelta = 1.0f/60.0f) {
			auto a = target - current;
			float magnitude = a.length();
			if (magnitude <= maxDistanceDelta || magnitude == 0.0f)
			{
				return target;
			}
			return current + a / magnitude * maxDistanceDelta;

		}
	};
}


#endif