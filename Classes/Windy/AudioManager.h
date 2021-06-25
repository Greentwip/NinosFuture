#include <string>
#include <map>

namespace windy {

	enum class Sounds {
		Opening,
		GAME,
		VICTORY,
		LOSE,
		WRONG_MOVE,
		COMBO_1,
		COMBO_2,
		COMBO_3,
		COMBO_4,
		COMBO_5,
		DELICIOUS,
		NONE
	};


	class AudioManager {
	private:
		static std::map<Sounds, std::string> soundsKeyMap;

	public:
		static void playBGM(Sounds, bool loop = false);
		static void playSFX(Sounds resource, bool loop = false);
		static void stopAll();
		static Sounds getCurrentTrack();

	private:
		static Sounds currentTrack;
		static int currentSfxId;
	};
}
