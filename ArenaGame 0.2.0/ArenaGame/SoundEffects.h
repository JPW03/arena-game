#pragma once
#include "FileCache.h"
#include <SFML/Audio.hpp>


//Static class containing all the sound effects to be used in other classes

class SoundEffects {

public:

	static sf::Sound PLAYER_HITSOUND;
	static sf::Sound WAVE_COMPLETE;
	static sf::Sound GAME_OVER;
	static sf::Sound BUTTON_SELECT;
	static sf::Sound VICTORY_FANFARE;

};