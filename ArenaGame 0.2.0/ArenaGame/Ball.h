#pragma once
#include "BallAnimation.h"
#include "Entity.h"
#include "Player.h"



//Class for the enemy type "Ball"

class Ball : public Entity {

public:

	Ball(Arena* arena);
	~Ball();

	enum State { Idle, Charging, Cooldown };
	enum VDirection { Up, Down };	//Vertical direction
	enum StuckDirection { None, sLeft, sRight, sUp, sDown };	//The direction needed to move to correct being stuck behind a platform

	Entity::Type GetType();

	void Update(float elapsedTime, sf::Vector2f playerPos);		//Update state if needed, then direct to suitable state.
	void Draw(sf::RenderWindow& window);

	void Move(float deltaX, float deltaY);
	void SetPosition(float x, float y);

	void PauseTimers();
	void ResumeTimers();

private:

	BallAnimation _animation;
	sf::Vector2f _playerPos;
	VDirection _vDirection;
	StuckDirection _stuckDirection;
	State _state;

	const float _maxVelocity;
	float _acceleration;
	bool _stuck;

	float _maxProjectileVel;
	struct Projectile {
		Projectile(float xVel, float yVel);
		~Projectile();

		sf::Sprite* sprite;
		float xVelocity;
		float yVelocity;
		
		//Animations
		int frame;
		Timer animationTimer;
	};

	std::list<Projectile*> _projectiles;
	void ShootProjectile(Projectile* projectile);
	bool ProjectileCollision(Projectile* projectile, Platform* platform);
	void UpdateProjectiles();

	Timer _cooldownTimer;

	//Collision functions
	void HandleMovement();
	void UpdateHurtboxes();
	void UpdateHitboxes();
};