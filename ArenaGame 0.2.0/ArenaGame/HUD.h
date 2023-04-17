#pragma once
#include <SFML/Graphics.hpp>




class HUD {

public:

	HUD();
	~HUD();

	void DrawHUD(sf::RenderWindow& window, int playerHealth, int wave);

private:

	const sf::Color HEALTH_COLOUR = sf::Color(160, 40, 40, 190);	//Slightly transparent dark red
	sf::Font _font;

	//HUD objects
	sf::RectangleShape _healthBar;
	sf::Text _waveDisplay;

};