#ifndef __GAME_TAGS_H__
#define __GAME_TAGS_H__

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
			Checkpoint,
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
		public:
			enum Checkpoint {
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

		enum Gui {
			Interface = 22,
			BarMeter
		};

		enum Scenery {
			Particle = 24,
			SpriteAnimation
		};
		
	};
}

#endif
