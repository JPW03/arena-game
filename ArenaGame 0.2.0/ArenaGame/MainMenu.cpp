#include "MainMenu.h"
#include <iostream>

//Initialise static variables
bool MainMenu::_shown = false;
sf::Texture& MainMenu::_menuTexture = *FileCache::GetArenaTexture("Textures/MainMenu.png");
sf::Sprite MainMenu::_menuSprite = sf::Sprite(_menuTexture);

MainMenu::Button MainMenu::_newGame(sf::IntRect(67, 214, 227, 51), MainMenu::Result::NewGame);
MainMenu::Button MainMenu::_loadGame(sf::IntRect(67, 287, 227, 51), MainMenu::Result::LoadGame);
MainMenu::Button MainMenu::_exit(sf::IntRect(67, 430, 227, 51), MainMenu::Result::Exit);
MainMenu::Button MainMenu::_options(sf::IntRect(), MainMenu::Result::Options);


//Show the main menu and await a result
MainMenu::Result MainMenu::AwaitResult(sf::RenderWindow& window) {

	//Draw the main menu to the window
	window.clear();
	_menuSprite.setPosition(0, 0);
	window.draw(_menuSprite);
	window.display();

	sf::Event event;
	window.pollEvent(event);

	//When the left mouse button is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition();

		//Correct mouse position to be inside the window
		mousePos.x -= window.getPosition().x + 7;
		mousePos.y -= window.getPosition().y + 30;
		//"+7" and "+30" accounts for the top bar

		//std::cout << "Pressed pos: " << mousePos.x << ", " << mousePos.y << "\n";

		//Check if the position of the cursor is inside a button
		if (_newGame.rect.contains(mousePos)) {
			return Result::NewGame;
		}
		else if (_loadGame.rect.contains(mousePos)) {
			return Result::LoadGame;
		}
		else if (_exit.rect.contains(mousePos)) {
			return Result::Exit;
		}
	}
	return Result::Nothing;
}


MainMenu::Button::Button(sf::IntRect buttonRect, MainMenu::Result buttonAction) :
rect(buttonRect), action(buttonAction) {}