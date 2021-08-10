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
			Ladder
		};

		enum Weapon {
			WeaponPlayer = 14,
			WeaponEnemy,
			WeaponNone
		};

		class Logic {
		public:
			enum Checkpoint {
				First = 17,
				Last
			};
		};
		
		enum Actions {
			Animation = 19,
			Color,
			Visibility 
		};

		enum Camera {
			FreeScroll = 22
		};

		enum Gui {
			Interface = 23,
			BarMeter
		};

		enum Scenery {
			Particle = 25,
			SpriteAnimation
		};
		
	};
}

#endif
