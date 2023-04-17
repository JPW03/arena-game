#pragma once
#include "Arena.h"
#include "FileCache.h"
#include "Timer.h"
#include <algorithm>
#include <list>
#include "SFML/Graphics.hpp"
#include "SoundEffects.h"



//Base class for all entities in the game (player and enemies)

class Entity {

public:
	Entity(Arena* arena);
	Entity();
	~Entity();

	enum Direction { Left, Right };		//Where the player is facing, used for movement
	
	enum Type { None, Player, Ball };	//Used to check what type of entity an instance is
	virtual Type GetType();

	virtual void Update(float elapsedTime);
	virtual void Draw(sf::RenderWindow& window);

	virtual void Move(float deltaX, float deltaY);
	virtual void SetPosition(float x, float y);

	sf::Vector2f GetPosition();
	sf::Vector2f* GetPositionCache();
	sf::Sprite* GetSprite();
	float GetWidth();
	float GetHeight();
	std::list<sf::FloatRect> GetHurtboxes();
	std::list<sf::FloatRect> GetHitboxes();

	int GetHealth();
	void SetHealth(int health);
	void TakeDamage(int damage);
	void TakeDamage(int damage, int comboValue);	//For enemies taking damage from the player
	int GetDamage();

	virtual void PauseTimers();
	virtual void ResumeTimers();

	void ToggleShowCollisionBoxes();

protected:

	sf::Sprite _sprite;
	Direction _direction;
	Direction _prevDirection;	//For detecting direction changes
	float _yVelocity;	//pixels per second
	float _xVelocity;
	int _health;
	int _damage;	//The amount of damage the entity does at any moment.
	int _recievedCombo;

	//Collision booleans
	bool _colliding;
	bool _topColliding;		//If true, velocity in the corresponding direction will be prevented
	bool _bottomColliding;
	bool _rightColliding;
	bool _leftColliding;
	bool _ignoresThinPlatforms;	//If true, the entity will phase through all sides of a thin platform

	//Collision functions
	void HandleCollision(std::list<sf::FloatRect> hurtboxes);
	void Collision(Platform* platform, std::list<sf::FloatRect> hurtboxes);
	void ResolveCollision(float contactTime, sf::Vector2i contactNormal);
	virtual void UpdateHurtboxes();
	virtual void UpdateHitboxes();
	
	virtual void HandleMovement();
	//Format should go: UpdatePositionCache() -> UpdatePlatformCache() -> UpdateHurtboxes() -> HandleCollision() -> Move()
	//Needs to be this way in derived classes so derived versions of UpdateHurtboxes()/UpdateHitboxes() gets used

	Arena* _presentArena;
	struct touchingPlatform {
		touchingPlatform(Platform*, sf::Vector2f, sf::Vector2i, float);
		touchingPlatform();

		enum Direction { None, Top, Bottom, Left, Right };
		Direction side;
		Platform* platformPtr;
		sf::Vector2i contactNormal;	//Can be used to indicate direction
		sf::Vector2f contactPoint;
		float contactTime;
	};
	std::list<touchingPlatform> _currentPlatforms;	//Stores the platforms collided with in the previous update
	std::list<touchingPlatform> _previousPlatforms;	//Stores the previous 5 platforms collided with.
	void UpdatePlatformCache();

	//Collision boxes, will update position as the program runs,
	//	unlike PlayerAnimation collision boxes which are constant
	std::list<sf::FloatRect> _hitboxes;		//Where the player can hit
	std::list<sf::FloatRect> _hurtboxes;		//Where the player gets hit

	float _updateElapsed;	//Time taken to complete the previous update

	sf::Vector2f _positionCache[20];
	void UpdatePositionCache();

	//Invinsibility after taking damage
	bool _invinsible;
	Timer _invinsibilityTime;
	//When an entity is invinsible it won't take damage for 2 seconds but also can't attack for 2 seconds

	//Debug
	bool _showCollisionBoxes;
};