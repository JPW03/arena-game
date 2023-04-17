#pragma once
#include "Arena.h"
#include "Timer.h"
#include "EntityManager.h"
#include "SoundEffects.h"
#include "HUD.h"


//Game object:
//	- Manages the enemy waves
//	- Directs the code to drawing and updating each entity
//	- Loads the arena
//	- Manages transissions between waves or exiting/entering a game instance

class Game {

public:

	static void NewGame();
	static void LoadGame(int waveNumber);
	static void UnloadGame();

	enum State { WaveEnded, WaveOngoing, Paused, PlayerDead, WaveStarting, Test, Unloaded, Victory };
	//Test state is is just the player being able to move around.

	//Each wave contains an int of the number of each enemy type to spawn in that wave.
	struct Wave {
		Wave();
		Wave(int balls);
		~Wave();

		int TotalEnemies();

		int balls;
	};

	static void Draw(sf::RenderWindow& window);
	static void Update();

	static void PauseTimers();
	static void ResumeTimers();

	static int GetCurrentWave();

	//True if the game is running
	static bool Loaded;

private:
	
	static void Initialise();

	static Arena* _arena;
	static EntityManager* _entityManager;
	static Wave _waves[10];
	static int _currentWave;
	static Player* _player;
	static State _state;
	static HUD _hud;

	//Timers
	static Timer _waveTimer;	//Keeps track of how long the wave has lasted.
	static float _waveElapsed;
};