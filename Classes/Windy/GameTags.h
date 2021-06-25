#ifndef GAME_TAGS_H
#define GAME_TAGS_H

namespace windy {
	class GameTags {
	public:
		enum General {
			None = -1,
			Player = 1,
			Item,
			Enemy,
			Block,
			Camera,
			Scroll,
			CheckPoint,
			Teleporter,
			Bounds,
			Hole,
			Door,
			Ladder
		};

		enum Weapon {
			WeaponPlayer = 13,
			WeaponEnemy,
			WeaponNone
		};

		class Logic {
			enum CheckPoint {
				First = 16,
				Last
			};
		};
		
		enum Actions {
			Animation = 18,
			Color,
			Visibility = 21
		};

		enum Camera {
			FreeScroll = 20
		};
		
	};
}

#endif
