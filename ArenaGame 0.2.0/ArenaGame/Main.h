#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <iostream>
#include "Timer.h"
#include "Game.h"
#include "MainMenu.h"


class Main {

public:

	enum AppState { Closing, GameRunning, MainMenu, Options, Paused };

	static void Start();
	static void MainLoop();

	//Keybind get function
	enum KeyBinds { Up, Down, Left, Right, Jump, Dash, Neutral, Side, UpAtk, DownAtk };
	static sf::Keyboard::Key GetKeybind(KeyBinds);

	const bool DebugMode = true;

private:

	static int GetSavedWave();
	static void SaveWave(int waveNumber);
	static void ProcessMainMenu();

	static sf::RenderWindow _window;

	static AppState _appState;

	//Bools
	static bool _gameStarted;
	static bool _focus;

	//App settings
	static sf::Vector2f _windowSize;
	static bool _vSync;
	static bool _fullscreen;
	static int _frameRateLimit;	//If equal to 0, the framerate is uncapped

	//Keybinds
	static sf::Keyboard::Key _inputUp;
	static sf::Keyboard::Key _inputDown;
	static sf::Keyboard::Key _inputLeft;
	static sf::Keyboard::Key _inputRight;
	static sf::Keyboard::Key _inputJump;
	static sf::Keyboard::Key _inputDash;
	static sf::Keyboard::Key _inputNeutral;
	static sf::Keyboard::Key _inputSide;
	static sf::Keyboard::Key _inputUpAtk;
	static sf::Keyboard::Key _inputDownAtk;
};