#include "Main.h"
#include "windows.h"
#include <fstream>

int main() {
	//Push the code into the main object
	Main::Start();

	return 0;
}



//Initialise static Main variables
sf::Vector2f Main::_windowSize = sf::Vector2f(1000, 700);
bool Main::_vSync = false;
bool Main::_fullscreen = false;
int Main::_frameRateLimit = 60;

sf::RenderWindow Main::_window;
bool Main::_focus = true;
Main::AppState Main::_appState = MainMenu;

sf::Keyboard::Key Main::_inputUp = sf::Keyboard::W;
sf::Keyboard::Key Main::_inputDown = sf::Keyboard::S;
sf::Keyboard::Key Main::_inputRight = sf::Keyboard::D;
sf::Keyboard::Key Main::_inputLeft = sf::Keyboard::A;
sf::Keyboard::Key Main::_inputJump = sf::Keyboard::Space;
sf::Keyboard::Key Main::_inputDash = sf::Keyboard::LShift;
sf::Keyboard::Key Main::_inputNeutral = sf::Keyboard::J;
sf::Keyboard::Key Main::_inputSide = sf::Keyboard::L;
sf::Keyboard::Key Main::_inputUpAtk = sf::Keyboard::I;
sf::Keyboard::Key Main::_inputDownAtk = sf::Keyboard::K;


//Initialise the app
void Main::Start() {
	//This function was originally inside of main(), but it was easier to push it into the Main object function instead of just typing "Main::" in front of every variable


	//Read config file (implement later)


	//Create the window
	_window.create(sf::VideoMode(_windowSize.x, _windowSize.y), "ARENAGAME");
	
	if (_vSync) {
		_window.setVerticalSyncEnabled(_vSync);
	}
	else {
		_window.setVerticalSyncEnabled(false);
		_window.setFramerateLimit(_frameRateLimit);
	}

	//Create the icon of the window
	sf::Image icon;
	icon.loadFromFile("Textures/icon.png");
	_window.setIcon(48, 48, icon.getPixelsPtr());


	//Main app loop
	while (_window.isOpen()) {

		//App will pause if the window is not in focus
		/*if (!_window.hasFocus()) {
			_focus = false;
			if (Game::Loaded) {
				Game::PauseTimers();
			}
			continue;
		}*/


		if (true) {
			MainLoop();
			_focus = true;
		}
	}
}



void Main::MainLoop() {
	sf::Event currentEvent;
	_window.pollEvent(currentEvent);

	if (currentEvent.type == sf::Event::Closed) {
		_appState = Closing;
	}


	switch (_appState) {
		
	case (AppState::GameRunning):
		_window.clear(sf::Color(100, 100, 100, 255));

		//The game should be loaded by this point but if not...
		if (!Game::Loaded) {
			Game::NewGame();
			//throw(std::runtime_error("Main::MainLoop(): Game not loaded."));
		}

		//Check if the game is being paused (temporary; add proper pause menu)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			SaveWave(Game::GetCurrentWave());
			_appState = MainMenu;
		}

		Game::Update();

		Game::Draw(_window);

		_window.display();
		break;


	case (AppState::Closing):
		_window.close();
		break;


	case (AppState::MainMenu):
		ProcessMainMenu();
		break;


	case (AppState::Options):
		break;
	}
}



sf::Keyboard::Key Main::GetKeybind(Main::KeyBinds keybind) {
	switch (keybind) {
	case (Up):
		return _inputUp;
	case (Down):
		return _inputDown;
	case (Left):
		return _inputLeft;
	case (Right):
		return _inputRight;
	case (Jump):
		return _inputJump;
	case (Dash):
		return _inputDash;
	case (Neutral):
		return _inputNeutral;
	case (Side):
		return _inputSide;
	case (UpAtk):
		return _inputUpAtk;
	case (DownAtk):
		return _inputDownAtk;
	}
}



//Returns the wave number saved in save.txt
int Main::GetSavedWave() {
	std::ifstream saveFile("Saves/save.txt");
	std::string waveStr;

	std::getline(saveFile, waveStr);

	saveFile.close();
	return std::stoi(waveStr);
}

//Saves the wave number to save.txt
void Main::SaveWave(int waveNumber) {
	std::ofstream saveFile("Saves/save.txt");
	std::string waveStr = std::to_string(waveNumber);

	saveFile << waveStr;
	saveFile.close();
}


//Checks the outcome of the main menu and changes the app state if needed
void Main::ProcessMainMenu() {
	MainMenu::Result result = MainMenu::AwaitResult(_window);

	if (result != MainMenu::Result::Nothing) {
		SoundEffects::BUTTON_SELECT.play();
		switch (result) {
		case (MainMenu::Result::NewGame):
			Game::NewGame();
			_appState = GameRunning;
			break;
		case (MainMenu::Result::LoadGame):
			Game::LoadGame(GetSavedWave());
			_appState = GameRunning;
			break;
		case (MainMenu::Result::Exit):
			_appState = Closing;
			break;
		case (MainMenu::Result::Options):
			_appState = Options;
			break;
		}
		result = MainMenu::AwaitResult(_window);
	}
}