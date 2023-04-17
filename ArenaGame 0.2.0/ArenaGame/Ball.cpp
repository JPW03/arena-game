#include "Ball.h"
#include <iostream>


Ball::Ball(Arena* arena) :
	Entity(arena),
	_animation(BallAnimation(&_sprite)),
	_maxVelocity(50.f),
	_acceleration(25.f),
	_stuck(false),
	_vDirection(VDirection::Down),
	_state(Cooldown) {

	//Protected variables
	_direction = Right;
	_health = 200;
	_ignoresThinPlatforms = true;
	_damage = 15.f;
	
	_animation.SetFrame("Idle");

	_sprite.setOrigin(_animation.spriteOrigin);

	_sprite.setPosition(arena->GetEnemySpawn());

	_cooldownTimer.Restart();
}

Ball::~Ball() {}



Ball::Projectile::Projectile(float xVel, float yVel) :
	xVelocity(xVel), yVelocity(yVel), frame(0) {}

Ball::Projectile::~Projectile() {}



void Ball::Update(float elapsedTime, sf::Vector2f playerPos) {
	_playerPos = playerPos;
	_updateElapsed = elapsedTime;
	//_updateElapsed = 0.166666f;

	//Check if the invinsibility frames have ran out
	if (_invinsible) {
		if (_invinsibilityTime.GetElapsedTime() >= 2.f) {
			_invinsible = false;
			_sprite.setScale(1, 1);
			_recievedCombo = 0;
		}
		_animation.ShowInvinsibility();
	}



	sf::Vector2f ballPos = _sprite.getPosition();

	//Calculate the angle between the ball and the player, with respect to the positive x (tan x = opp / adj)
	float angle = atanf((_playerPos.y - ballPos.y) / (_playerPos.x - ballPos.x));

	//The hypotenuse is equal to the maximum velocity.
	//Use trig to get one of the velocities (like a line in a right angled triangle)
	_xVelocity = _maxVelocity * cos(angle);
	//Use pythagoras to get the other velocity
	_yVelocity = sqrt(pow(_maxVelocity, 2) - pow(_xVelocity, 2));

	//Move the ball in the direction of the player
	if (ballPos.x < _playerPos.x) {
		_xVelocity = _xVelocity;
		_direction = Entity::Right;
	}
	else if (ballPos.x > _playerPos.x) {
		_xVelocity = -_xVelocity;
		_direction = Entity::Left;
	}

	if (ballPos.y < _playerPos.y) {
		_yVelocity = _yVelocity;
		_vDirection = VDirection::Down;
	}
	else if (ballPos.y > _playerPos.y) {
		_yVelocity = -_yVelocity;
		_vDirection = VDirection::Up;
	}


	
	//Shoot projectiles towards the player
	switch (_state) {

	//Wait before being able to shoot projectile
	case (Cooldown):
		if (_cooldownTimer.GetElapsedTime() >= 5.f) {
			_state = Charging;
		}
		break;


	//Go through charging animation, then create the projectile
	case (Charging):
		_animation.NextCharging();
		
		//If on the final charging animation frame, fire
		if (_animation.GetFrame()->name == "Idle") {
			//The velocity of a projectile is 60% faster than the ball
			Projectile* temp = new Projectile(_xVelocity * 1.6, _yVelocity * 1.6);

			ShootProjectile(temp);
			_state = Cooldown;
			_cooldownTimer.Restart();
		}
		break;
	}



	//If the ball is stuck against a platform, the ball will move faster in the direction of the closest platform edge

	//If initially getting stuck
	if (!_stuck) {
		for (const auto& platform : _currentPlatforms) {
			_stuck = true;
			sf::FloatRect rect = platform.platformPtr->GetBoundingRect();

			//Horizontal
			if (platform.side == Entity::touchingPlatform::Top || platform.side == Entity::touchingPlatform::Bottom) {

				//Set the direction needed to get the entity unstuck based on the player's current position
				switch (_direction) {
				case (Entity::Right):
					_stuckDirection = sRight;
					break;
				case (Entity::Left):
					_stuckDirection = sLeft;
					break;
				}
			}


			//Vertical
			if (platform.side == Entity::touchingPlatform::Right || platform.side == Entity::touchingPlatform::Left) {

				//Set the direction needed to get the entity unstuck based on the player's current position
				switch (_vDirection) {
				case (VDirection::Up):
					_stuckDirection = sUp;
					break;
				case (VDirection::Down):
					_stuckDirection = sDown;
					break;
				}
			}
		}
	}

	//If stuck
	if (_stuck) {

		switch (_stuckDirection) {

		case (StuckDirection::sRight):
			_xVelocity = _maxVelocity;

			if (_leftColliding) {
				_stuckDirection = sLeft;
			}

			if (!_topColliding && !_bottomColliding) {
				_stuck = false;
			}
			break;

		case (StuckDirection::sLeft):
			_xVelocity = -_maxVelocity;

			if (_rightColliding) {
				_stuckDirection = sRight;
			}

			if (!_topColliding && !_bottomColliding) {
				_stuck = false;
			}
			break;

		case (StuckDirection::sDown):
			_yVelocity = _maxVelocity;

			if (_topColliding) {
				_stuckDirection = sUp;
			}

			if (!_leftColliding && !_rightColliding) {
				_stuck = false;
			}
			break;

		case (StuckDirection::sUp):
			_yVelocity = -_maxVelocity;

			if (_bottomColliding) {
				_stuckDirection = sDown;
			}

			if (!_leftColliding && !_rightColliding) {
				_stuck = false;
			}
			break;

		default:
			_stuck = false;
			break;

		}
	}

	//Correct velocities
	/*if (_xVelocity > _maxVelocity) {
		_xVelocity = _maxVelocity;
	}
	if (_xVelocity < -_maxVelocity) {
		_xVelocity = -_maxVelocity;
	}
	if (_yVelocity > _maxVelocity) {
		_yVelocity = _maxVelocity;
	}
	if (_yVelocity < -_maxVelocity) {
		_yVelocity = -_maxVelocity;
	}*/


	UpdateProjectiles();

	HandleMovement();
}


void Ball::ShootProjectile(Projectile* projectile) {
	projectile->sprite = _animation.NewProjectile();

	//Spawn the projectile at the centre of the ball
	projectile->sprite->setOrigin(11, 11);
	projectile->sprite->setPosition(_sprite.getPosition());

	//Add the projectile to the list.
	_projectiles.push_back(projectile);
}


void Ball::UpdateProjectiles() {
	//Create temporary list that stores the projectiles to be deleted this update
	std::list<Projectile*> toDelete;
	
	//Update each projectile
	for (auto& projectile : _projectiles) {

		//Update animation
		_animation.NextProjectile(projectile->sprite, projectile->frame, projectile->animationTimer);

		//Check collision
		bool collision = false;
		for (int i = 0; i < _presentArena->NoOfPlatforms(); i++) {
			Platform* temp = _presentArena->GetPlatformByIndex(i);

			//Projectiles pass through thin platforms
			if (temp->GetType() == Platform::PlatformType::Thin) continue;

			//If there was a collision, break the loop
			if (ProjectileCollision(projectile, temp)) {
				collision = true;
				//Add projectile to deletion list
				toDelete.push_back(projectile);
				break;
			}
		}

		//Move the projectile
		if (!collision)
			projectile->sprite->move(projectile->xVelocity * _updateElapsed, projectile->yVelocity * _updateElapsed);
	}

	//Delete each projectile in the temporary list
	for (auto& projectile : toDelete) {
		_projectiles.remove(projectile);
	}
}


bool Ball::ProjectileCollision(Projectile* projectile, Platform* platform) {
	//Just a trimmed down version of the regular ray based collision

	sf::FloatRect projRect = projectile->sprite->getGlobalBounds();

	//Expand the bounding box of the platform to account for the hurtbox's origin (top left corner)
	sf::FloatRect expandedPlatRect = platform->GetBoundingRect();
	expandedPlatRect.left -= projRect.width / 2;
	expandedPlatRect.top -= projRect.height / 2;
	expandedPlatRect.width += projRect.width;
	expandedPlatRect.height += projRect.height;

	//Calculate ray origin (centre of hurtbox) and ray direction vectors
	sf::Vector2f rayOrigin = { projRect.left + projRect.width / 2, projRect.top + projRect.height / 2 };
	sf::Vector2f rayDirection = { projectile->xVelocity * _updateElapsed, projectile->yVelocity * _updateElapsed };


	//Calculate times to intersections with platform's bounding axes

	sf::Vector2f tNear = sf::Vector2f(	//Time to first intersection of each axes (entering the platform)
		(expandedPlatRect.left - rayOrigin.x) / rayDirection.x,
		(expandedPlatRect.top - rayOrigin.y) / rayDirection.y);		//Relative to top left corner of platform

	sf::Vector2f tFar = sf::Vector2f(	//Time to last intersection of each axes (exiting the platform)
		(expandedPlatRect.left + expandedPlatRect.width - rayOrigin.x) / rayDirection.x,
		(expandedPlatRect.top + expandedPlatRect.height - rayOrigin.y) / rayDirection.y);		//Relative to bottom right corner of platform

	if (std::isnan(tFar.y) || std::isnan(tFar.x) || std::isnan(tNear.x) || std::isnan(tNear.y)) return false;

	//Sort distances
	if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
	if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

	//Reject if the ray doesnt collide.
	//	If both near coordinates are less than both far coordinates, then there is a collision.
	if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

	//Time to first intersection (when the ray has intersected both axes)
	//Time is measured relative to the ray (0 at origin, 1 at end, 0.5 half way, etc)
	float tHitNear = std::max(tNear.x, tNear.y);

	//Time to last intersection (when the ray leaves one of the axes)
	float tHitFar = std::min(tFar.x, tFar.y);

	// Reject if ray direction is pointing away from object
	if (tHitFar < 0) return false;

	//If the ray collides within the distance of the ray, a collision has occured
	if (tHitNear >= 0.f && tHitNear < 1.f) {

		//Remove the projectile from the list
		//_projectiles.remove(projectile);
		return true;
	}
	return false;
}



void Ball::Move(float deltaX, float deltaY) {
	Entity::Move(deltaX, deltaY);
}

void Ball::SetPosition(float x, float y) {
	Entity::SetPosition(x, y);
}


void Ball::HandleMovement() {
	UpdatePositionCache();

	//Update touchingPlatform cache (the platforms previously collided with)
	UpdatePlatformCache();

	UpdateHurtboxes();	//Update hurtboxes just before _currentPlatforms clears
	UpdateHitboxes();

	HandleCollision(_hurtboxes);

	//Move the player.
	Move(_xVelocity * _updateElapsed, _yVelocity * _updateElapsed);
}


void Ball::UpdateHurtboxes() {
	_hurtboxes = _animation.GetFrame()->hurtboxes;
	Entity::UpdateHurtboxes();
}

void Ball::UpdateHitboxes() {
	_hitboxes = _animation.GetFrame()->hitboxes;
	Entity::UpdateHitboxes();

	//Add the bounding boxes of each projectile to the hitbox list
	for (auto& projectile : _projectiles) {
		_hitboxes.push_back(projectile->sprite->getGlobalBounds());
	}
}



void Ball::Draw(sf::RenderWindow& window) {
	Entity::Draw(window);

	for (const auto& projectile : _projectiles) {
		window.draw(*projectile->sprite);
	}
}


void Ball::PauseTimers() {
	_cooldownTimer.Pause();
}

void Ball::ResumeTimers() {
	_cooldownTimer.Resume();
}


Entity::Type Ball::GetType() {
	return Entity::Type::Ball;
}