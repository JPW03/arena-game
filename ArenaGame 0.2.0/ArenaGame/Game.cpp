#include "Game.h"
#include <fstream>
#include <iostream>



Game::Wave::Wave(int balls) :
	balls(balls) {}

Game::Wave::Wave() {}

Game::Wave::~Wave() {}

int Game::Wave::TotalEnemies() {
	return balls;
}

Game::Wave Game::_waves[10] = {
	Wave(1),
	Wave(3),
	Wave(4),
	Wave(5),
	Wave(5),
	Wave(5),
	Wave(5),
	Wave(7),
	Wave(7),
	Wave(10),
};



//Starting game functions

void Game::NewGame() {
	_currentWave = 0;
	Initialise();
}

void Game::LoadGame(int waveNumber) {
	_currentWave = waveNumber;
	Initialise();
}

void Game::Initialise() {
	_arena = new Arena("Arenas/Arena1.arena");

	//Create entity manager (also spawns player)
	_entityManager = new EntityManager(_arena);

	_state = WaveStarting;
	_player = _entityManager->GetPlayer();
	_waveTimer.Restart();

	Loaded = true;
}



void Game::Update() {

	//Check if the player is dead
	if (_entityManager->GetPlayer()->GetHealth() <= 0) {
		_state = PlayerDead;
	}

	//Update entities
	_entityManager->UpdateAll();

	switch (_state) {

	case (WaveOngoing):
		//Check if no enemies are alive/wave has ended
		if (_entityManager->GetEnemyCount() == 0) {
			SoundEffects::WAVE_COMPLETE.play();
			_state = WaveEnded;
			_waveElapsed = _waveTimer.GetElapsedTime();
			_waveTimer.Restart();
		}
		break;

	case (WaveEnded):
		//Wait 5 seconds before starting the next wave.
		if (_waveTimer.GetElapsedTime() >= 5.f) {
			_currentWave++;
			if (_currentWave >= 10) {	//The game has been won
				_state = Victory;
				SoundEffects::VICTORY_FANFARE.play();
			}
			else {
				_state = WaveStarting;
				_waveTimer.Restart();
				_entityManager->RefillPlayerHealth();
			}
		}
		break;

	case (WaveStarting):

		//Spawn 1 enemy every 3 seconds.
		if (_waveTimer.GetElapsedTime() >= 4.f) {

			//Randomly pick an enemy type:
			//There's only 1 enemy type right now so...

			if (_entityManager->GetBallCount() < _waves[_currentWave].balls) {
				_entityManager->SpawnBall();
			}

			_waveTimer.Restart();
		}

		//Once all enemies have been spawned
		if (_entityManager->GetEnemyCount() == _waves[_currentWave].TotalEnemies()) {
			_state = WaveOngoing;
		}
		break;

	case (PlayerDead):
		_entityManager->~EntityManager();
		_waveTimer.Restart();
		SoundEffects::GAME_OVER.play();
		
		//Wait 3 seconds before spawning the player back in
		while (_waveTimer.GetElapsedTime() < 3.f) {
			//Could probably draw a "Game Over" image onto the screen
			int stallingForTime = 1 + 1;
		}

		if (_waveTimer.GetElapsedTime() >= 3.f) {
			_entityManager = new EntityManager(_arena);
		}

		_state = WaveStarting;
		break;

	case (Test):
		break;

	case (Victory):
		break;

	}

}



void Game::Draw(sf::RenderWindow& window) {
	_arena->Draw(window);

	if (_state != PlayerDead) {
		_entityManager->DrawAll(window);
	}

	//Draw HUD
	_hud.DrawHUD(window, _entityManager->GetPlayer()->GetHealth(), _currentWave);
}



void Game::PauseTimers() {
	_waveTimer.Pause();
	_entityManager->PauseTimers();
}

void Game::ResumeTimers() {
	_waveTimer.Resume();
	_entityManager->ResumeTimers();
}



void Game::UnloadGame() {
	delete _entityManager;
	delete _arena;
	_state = Unloaded;

	//Save the current wave value into the save file
}


int Game::GetCurrentWave() {
	return _currentWave;
}



//Initialise static variables

Arena* Game::_arena = new Arena("Arenas/Arena1.arena");
EntityManager* Game::_entityManager;
int Game::_currentWave;
Player* Game::_player;
Game::State Game::_state = Test;
Timer Game::_waveTimer;
float Game::_waveElapsed;
bool Game::Loaded = false;
HUD Game::_hud;