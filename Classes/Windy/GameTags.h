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
			Platform
		};

		enum Weapon {
			WeaponPlayer = 15,
			WeaponEnemy,
			WeaponNone
		};

		class Logic {
		public:
			enum Checkpoint {
				First = 18,
				Last
			};
		};
		
		enum Actions {
			Animation = 20,
			Color,
			Visibility 
		};

		enum Camera {
			FreeScroll = 23
		};

		enum Gui {
			Interface = 24,
			BarMeter
		};

		enum Scenery {
			Particle = 26,
			SpriteAnimation
		};
		
	};
}

#endif
