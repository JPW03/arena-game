#pragma once
#include <SFML/Graphics.hpp>
#include "FileCache.h"
#include "SoundEffects.h"


class MainMenu {

public:

	enum Result { Nothing, Exit, NewGame, LoadGame, Options };

	struct Button {
		Button(sf::IntRect, Result);

		sf::IntRect rect;
		Result action;
	};

	static Result AwaitResult(sf::RenderWindow& window);

private:

	static bool _shown;
	
	static Button _newGame;
	static Button _loadGame;
	static Button _exit;
	static Button _options;

	static sf::Texture& _menuTexture;
	static sf::Sprite _menuSprite;

};