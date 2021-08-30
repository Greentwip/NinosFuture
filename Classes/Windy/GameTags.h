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
			Boss,
			Block,
			Camera,
			Scroll,
			Checkpoint,
			Teleporter,
			Bounds,
			Hole,
			Door,
			Ladder,
			Platform,
			Water
		};

		enum Weapon {
			WeaponPlayer = 16,
			WeaponEnemy,
			WeaponNone
		};

		class Logic {
		public:
			enum Checkpoint {
				First = 19,
				Last
			};
		};
		
		enum Actions {
			Animation = 21,
			Color,
			Visibility 
		};

		enum Camera {
			FreeScroll = 24
		};

		enum Gui {
			Interface = 25,
			BarMeter
		};

		enum Scenery {
			Particle = 27,
			SpriteAnimation
		};
		
	};
}

#endif
