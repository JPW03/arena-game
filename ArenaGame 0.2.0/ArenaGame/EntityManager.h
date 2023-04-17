#pragma once
#include "Entity.h"
#include "Player.h"
#include "Ball.h"
#include "Arena.h"



class EntityManager {

public:
	EntityManager(Arena* arena);
	~EntityManager();

	void DrawAll(sf::RenderWindow& window);
	void UpdateAll();

	Player* GetPlayer();
	int GetEnemyCount();
	int GetBallCount();
	void SetPlayer(Player*);
	void RefillPlayerHealth();

	void HandleEntityCollisions();
	void EntityCollision(Entity* entity);

	void SpawnBall();

	void PauseTimers();
	void ResumeTimers();

	void ToggleShowCollisionBoxes();


private:

	Player* _player;
	Arena* _presentArena;

	//Store collision boxes of player so they don't have to be called upon for every entity collision
	std::list<sf::FloatRect> _playerHurtboxes;
	std::list<sf::FloatRect> _playerHitboxes;

	//List for each type of enemy
	std::list<Ball*> _balls;



	//Instead of having an update timer in each entity, just have 1 that's passed to every update function from the entity manager
	Timer _updateTime;
	float _updateElapsed;

	//Deallocator for the enemy vector
	/*struct EnemyDeallocator {
		void operator()(const std::list<Entity*> *p) {
			delete p;
		}
	};*/

	//Debug
	bool _showCollisionBoxes;
	bool _f5Pressed;
};