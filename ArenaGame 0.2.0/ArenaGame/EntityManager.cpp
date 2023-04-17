#include "EntityManager.h"


EntityManager::EntityManager(Arena* arena) :
	_presentArena(arena),
	_player(new Player(arena)), 
	_f5Pressed(false), _showCollisionBoxes(false) {

	_updateTime.Restart();
}

EntityManager::~EntityManager() {
	delete _player;
	//std::for_each(_balls.begin(), _balls.end(), EnemyDeallocator());
}



void EntityManager::DrawAll(sf::RenderWindow& window) {
	_player->Draw(window);

	for (const auto& ball : _balls) {
		ball->Draw(window);
	}
}

void EntityManager::UpdateAll() {
	_updateElapsed = _updateTime.GetElapsedTime();
	_updateTime.Restart();

	//Check if any hitboxes between entities have collided
	HandleEntityCollisions();

	//Call update functions
	_player->Update(_updateElapsed);
	for (const auto& ball : _balls) {
		ball->Update(_updateElapsed, _player->GetPosition());
	}

	//Check if any of the enemies have died and remove them from their respective lists
	std::list<Ball*> deleteTemp;
	for (auto ball : _balls) {
		if (ball->GetHealth() <= 0.f) {
			deleteTemp.push_back(ball);
			//_balls.remove(ball);
		}
	}
	for (auto& ball : deleteTemp) {
		_balls.remove(ball);
	}

	//Debug
	//Toggle show collision boxes
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) {
		if (!_f5Pressed) {
			_f5Pressed = true;
			ToggleShowCollisionBoxes();
		}
	}
	else {
		_f5Pressed = false;
	}
}

void EntityManager::HandleEntityCollisions() {

	//Get the player's collision boxes
	_playerHurtboxes = _player->GetHurtboxes();
	_playerHitboxes = _player->GetHitboxes();

	//Loop between each enemy's hitboxes
	for (const auto& ball : _balls) {
		EntityCollision(ball);
	}
}


void EntityManager::EntityCollision(Entity* enemy) {
	bool enemyHit = false;
	bool playerHit = false;

	//Get the enemy's collision boxes
	std::list<sf::FloatRect> enemyHurtboxes = enemy->GetHurtboxes();
	std::list<sf::FloatRect> enemyHitboxes = enemy->GetHitboxes();

	//Check for overlap between player hurtboxes and enemy hitboxes
	if (!playerHit) {
		for (const auto& pHurtbox : _playerHurtboxes) {
			for (const auto& eHitbox : enemyHitboxes) {
				if (pHurtbox.intersects(eHitbox)) {
					playerHit = true;
					_player->TakeDamage(enemy->GetDamage());
					break;
				}
			}

			//Player can also take damage from touching enemy hurtboxes
			if (!playerHit) {	//If not already hit
				for (const auto& eHurtbox : enemyHurtboxes) {
					if (pHurtbox.intersects(eHurtbox)) {
						playerHit = true;
						_player->TakeDamage(enemy->GetDamage());
						break;
					}
				}
			}

			if (playerHit)
				break;
		}
	}

	//Check for overlap between enemy hurtboxes and player hitboxes
	for (const auto& eHurtbox : enemyHurtboxes) {
		for (const auto& pHitbox : _playerHitboxes) {
			if (eHurtbox.intersects(pHitbox)) {
				enemyHit = true;
				enemy->TakeDamage(_player->GetDamage(), _player->GetComboValue());
				break;
			}
		}
		if (enemyHit)
			break;
	}
}


void EntityManager::ToggleShowCollisionBoxes() {
	_player->ToggleShowCollisionBoxes();
	for (const auto& ball : _balls) {
		ball->ToggleShowCollisionBoxes();
	}
}



void EntityManager::PauseTimers() {
	_updateTime.Pause();

	_player->PauseTimers();
	for (const auto& ball : _balls) {
		ball->PauseTimers();
	}
}

void EntityManager::ResumeTimers() {
	_updateTime.Resume();
	
	_player->ResumeTimers();
	for (const auto& ball : _balls) {
		ball->ResumeTimers();
	}
}


//Spawning enemy functions
void EntityManager::SpawnBall() {
	Ball* temp = new Ball(_presentArena);
	_balls.push_back(temp);
}





//Get and set methods

Player* EntityManager::GetPlayer() {
	return _player;
}

void EntityManager::SetPlayer(Player* newPlayer) {
	_player = newPlayer;
}

//Returns the number of enemies alive.
int EntityManager::GetEnemyCount() {
	return _balls.size();
}

//Returns the number of enemy balls alive
int EntityManager::GetBallCount() {
	return _balls.size();
}

void EntityManager::RefillPlayerHealth() {
	_player->SetHealth(100);
}