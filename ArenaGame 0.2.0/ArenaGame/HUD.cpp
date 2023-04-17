#include "HUD.h"



HUD::HUD() {

	//Construct health bar
	_healthBar = sf::RectangleShape(sf::Vector2f(30, 200));
	_healthBar.setPosition(10, 10);
	_healthBar.setFillColor(HEALTH_COLOUR);


	//Construct wave display
	if (!_font.loadFromFile("Textures/Microsoft Tai Le.ttf")) throw("Font failed to load.");

	_waveDisplay = sf::Text("Wave: ", _font, 30);
	_waveDisplay.setPosition(870, 3);
	_waveDisplay.setFillColor(sf::Color(0, 0, 0, 255));
}

HUD::~HUD() {}



void HUD::DrawHUD(sf::RenderWindow& window, int playerHealth, int wave) {

	//Correct health value (so the bar doesnt stretch backwards or cease to exist)
	if (playerHealth <= 0) {
		playerHealth = 1;
	}

	//Update and draw healthbar
	_healthBar.setSize(sf::Vector2f((float)2 * playerHealth, (float)30));
	window.draw(_healthBar);

	//Update and draw 
	_waveDisplay.setString("Wave: " + std::to_string(wave + 1));
	window.draw(_waveDisplay);
}