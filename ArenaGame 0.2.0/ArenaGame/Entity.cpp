#include "Entity.h"
#include <iostream>


Entity::Entity(Arena* arena) : 
	_presentArena(arena),
	_yVelocity(0.f), _xVelocity(0.f), _damage(0), _health(1) {

	//Fill position cache with empty vectors
	for (int i = 0; i < 10; i++) {
		_positionCache[i] = sf::Vector2f();
	}
}

Entity::Entity() {}

Entity::~Entity() {}



Entity::touchingPlatform::touchingPlatform(Platform* platform, sf::Vector2f ContactPoint, sf::Vector2i ContactNormal, float ContactTime) :
	platformPtr(platform), contactNormal(ContactNormal), contactPoint(ContactPoint), contactTime(ContactTime) {
	//Compute the contact normal to assign the platform a direction enum that the entity is colliding with
	switch (contactNormal.x) {
	case (0):
		switch (contactNormal.y) {
		case(0):
			side = touchingPlatform::None;
			break;
		case(1):
			side = touchingPlatform::Bottom;
			break;
		case(-1):
			side = touchingPlatform::Top;
		}
		break;
	case(-1):
		side = touchingPlatform::Left;
		break;
	case(1):
		side = touchingPlatform::Right;
		break;
	}
}



//Basic movement functions. Must be overridden in subclasses to add UpdateHurtbox/Hitbox functions

void Entity::Move(float deltaX, float deltaY) {
	_sprite.move(deltaX, deltaY);
}

void Entity::SetPosition(float x, float y) {
	_sprite.setPosition(x, y);
}



void Entity::UpdatePositionCache() {
	//Adds the newest position to the front of the array and moves other positions back 1 space.
	for (int i = 19; i > 0; i--) {
		_positionCache[i] = _positionCache[i - 1];
	}
	_positionCache[0] = GetPosition();
}



void Entity::HandleCollision(std::list<sf::FloatRect> hurtboxes) {
	//Reset collision booleans and platform vector
	_currentPlatforms.clear();
	_colliding = false, _topColliding = false, _bottomColliding = false, _leftColliding = false, _rightColliding = false;
	//Check if the player is colliding with a platform
	for (int i = 0; i < _presentArena->NoOfPlatforms(); i++) {
		Platform* temp = _presentArena->GetPlatformByIndex(i);
		
		//The platform is checked for being thin, in case this entity can ignore their collision
		if (_ignoresThinPlatforms && temp->GetType() == Platform::PlatformType::Thin) continue;

		Collision(temp, hurtboxes);
	}

	//Sort the collided platforms in order of contact time (how close it is to the player)
	/*std::sort(_currentPlatforms.begin(), _currentPlatforms.end(), [](const touchingPlatform& a, const touchingPlatform& b) {
		return a.contactTime < b.contactTime;
	});*/
	//	^ Not necessary when the velocities will change every update, and no 2 platforms' edges are directly next to each other

	//For each platform collided with...
	for (const auto& platform : _currentPlatforms) {
		//Move the entity back to the edge of the platform
		ResolveCollision(platform.contactTime, platform.contactNormal);
	}
}



void Entity::UpdatePlatformCache() {
	for (const auto& platform : _currentPlatforms) {

		//Check if this platform is already in the cache
		bool stored = false;
		for (const auto& prevPlat : _previousPlatforms) {
			if (platform.platformPtr == prevPlat.platformPtr) stored = true;
		}
		if (!stored)
			_previousPlatforms.push_back(platform);

		//Can only hold 5 platforms at a time.
		if (_previousPlatforms.size() > 5) {
			_previousPlatforms.pop_front();
		}
	}
}



void Entity::ResolveCollision(float contactTime, sf::Vector2i contactNormal) {

	//Calculate new velocities to ensure the entity doesn't move into the platform

	//Velocities are multiplied by contact normal, ensuring the right velocity gets effected.
	// Then the abs of the velocity * 1 - contact time gives the new velocity

	_yVelocity += contactNormal.y * std::abs(_yVelocity) * (1 - contactTime);
	_xVelocity += contactNormal.x * std::abs(_xVelocity) * (1 - contactTime);
}



//Ray based collision (based on javidx9's rectangle collision detection)
void Entity::Collision(Platform* platform, std::list<sf::FloatRect> hurtboxes) {

	//Make sure the entity is actually moving:
	if (_xVelocity == 0.f && _yVelocity == 0.f) {
		return;
	}

	for (const auto& hurtbox : hurtboxes) {

		//Expand the bounding box of the platform to account for the hurtbox's origin (top left corner)
		sf::FloatRect expandedPlatRect = platform->GetBoundingRect();
		expandedPlatRect.left -= hurtbox.width / 2;
		expandedPlatRect.top -= hurtbox.height / 2;
		expandedPlatRect.width += hurtbox.width;
		expandedPlatRect.height += hurtbox.height;

		sf::Vector2i contactNormal = { 0,0 };
		sf::Vector2f contactPoint = { 0,0 };

		//Calculate ray origin (centre of hurtbox) and ray direction vectors
		sf::Vector2f rayOrigin = { hurtbox.left + hurtbox.width / 2, hurtbox.top + hurtbox.height / 2 };
		sf::Vector2f rayDirection = { _xVelocity * _updateElapsed, _yVelocity * _updateElapsed };


		//Calculate times to intersections with platform's bounding axes

		sf::Vector2f tNear = sf::Vector2f(	//Time to first intersection of each axes (entering the platform)
			(expandedPlatRect.left - rayOrigin.x) / rayDirection.x,
			(expandedPlatRect.top - rayOrigin.y) / rayDirection.y);		//Relative to top left corner of platform

		sf::Vector2f tFar = sf::Vector2f(	//Time to last intersection of each axes (exiting the platform)
			(expandedPlatRect.left + expandedPlatRect.width - rayOrigin.x) / rayDirection.x,
			(expandedPlatRect.top + expandedPlatRect.height - rayOrigin.y) / rayDirection.y);		//Relative to bottom right corner of platform

		if (std::isnan(tFar.y) || std::isnan(tFar.x) || std::isnan(tNear.x) || std::isnan(tNear.y)) continue;

		//Sort distances
		if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
		if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

		//Reject if the ray doesnt collide.
		//	If both near coordinates are less than both far coordinates, then there is a collision.
		if (tNear.x > tFar.y || tNear.y > tFar.x) continue;

		//Time to first intersection (when the ray has intersected both axes)
		//Time is measured relative to the ray (0 at origin, 1 at end, 0.5 half way, etc)
		float tHitNear = std::max(tNear.x, tNear.y);

		//Time to last intersection (when the ray leaves one of the axes)
		float tHitFar = std::min(tFar.x, tFar.y);

		// Reject if ray direction is pointing away from object
		if (tHitFar < 0) continue;

		// Contact point of collision from parametric line equation
		contactPoint = rayOrigin + tHitNear * rayDirection;

		//If the ray collides within the distance of the ray, a collision has occured
		if (tHitNear >= 0.f && tHitNear < 1.f) {

			//Set the normal according to the direction of the ray and the position the platform was hit.
			if (tNear.x > tNear.y) {	//If the x bound was hit after y bound

				//Can't collide with the sides of thin platforms
				if (platform->GetType() == Platform::PlatformType::Thin) continue;

				if (1 / rayDirection.x < 0) {
					contactNormal = { 1, 0 };	//Right
					_rightColliding = true;
				}
				else {
					contactNormal = { -1, 0 };	//Left
					_leftColliding = true;
				}
			}
			else if (tNear.x < tNear.y) {	//If the y bound was hit after x bound
				if (1 / rayDirection.y < 0) {
					if (platform->GetType() == Platform::PlatformType::Thin) continue;
					contactNormal = { 0, 1 };	//Bottom
					_bottomColliding = true;
				}
				else {
					contactNormal = { 0, -1 };	//Top
					_topColliding = true;
				}
			}

			//Construct touchingPlatform struct
			touchingPlatform temp = touchingPlatform(platform, contactPoint, contactNormal, tHitNear);
			_currentPlatforms.push_back(temp);
			//std::cout << "\nCOLLISION: Platform at (" << temp.platformPtr->GetCentre().x << "," << temp.platformPtr->GetCentre().y << "), Normal (" << temp.contactNormal.x << "," << temp.contactNormal.y << "), Point (" << temp.contactPoint.x << "," << temp.contactPoint.y << "), Time = " << temp.contactTime << "\n";
		}
		break;
	}
}


void Entity::UpdateHurtboxes() {
	for (auto& hurtbox : _hurtboxes) {

		//Hurboxes will appear on different points of the sprite depending on direction sprite is facing
		switch (_direction) {

		case (Left):	//Default hurtbox direction
			//Top of hurtbox = hurtbox top offset + top of sprite
			hurtbox.top += GetPosition().y - _sprite.getOrigin().y;
			//Flip the left of the hurtbox to the other side of the sprite, then add left of sprite.
			hurtbox.left = (_sprite.getTextureRect().width - (hurtbox.left + hurtbox.width)) + GetPosition().x - (_sprite.getTextureRect().width - _sprite.getOrigin().x);
			break;

		case (Right):	//x of origin flipped to other side of sprite
			hurtbox.top += GetPosition().y - _sprite.getOrigin().y;
			hurtbox.left += GetPosition().x - _sprite.getOrigin().x;
			break;
		}
	}
}


void Entity::UpdateHitboxes() {
	for (auto& hitbox : _hitboxes) {

		//Hurboxes will appear on different points of the sprite depending on direction sprite is facing
		switch (_direction) {

		case (Left):	//Default hurtbox direction
			//Top of hurtbox = hurtbox top offset + top of sprite
			hitbox.top += GetPosition().y - _sprite.getOrigin().y;
			//Flip the left of the hurtbox to the other side of the sprite, then add left of sprite.
			hitbox.left = (_sprite.getTextureRect().width - (hitbox.left + hitbox.width)) + GetPosition().x - (_sprite.getTextureRect().width - _sprite.getOrigin().x);
			break;

		case (Right):	//x of origin flipped to other side of sprite
			hitbox.top += GetPosition().y - _sprite.getOrigin().y;
			hitbox.left += GetPosition().x - _sprite.getOrigin().x;
			break;
		}
	}
}






sf::Vector2f Entity::GetPosition() {
	return _sprite.getPosition();
}

sf::Vector2f* Entity::GetPositionCache() {
	return _positionCache;
}//For C++ reasons, can't conventionally return an array from a function, so have to return a pointer (but the array is technically a pointer already (I think))

float Entity::GetHeight() {
	return _sprite.getTextureRect().height;
}

float Entity::GetWidth() {
	return _sprite.getTextureRect().width;
}

sf::Sprite* Entity::GetSprite() {
	return &_sprite;
}

int Entity::GetHealth() {
	return _health;
}

void Entity::SetHealth(int health) {
	_health = health;
}

void Entity::TakeDamage(int damage) {
	if (!_invinsible) {
		_health -= damage;
		_invinsible = true;
		_invinsibilityTime.Restart();
	}
}

void Entity::TakeDamage(int damage, int comboValue) {
	if (comboValue > _recievedCombo) {
		_health -= damage;
		_recievedCombo = comboValue;
		_invinsible = true;
		_invinsibilityTime.Restart();
		SoundEffects::PLAYER_HITSOUND.play();
	}
	else {
		if (!_invinsible) {
			_health -= damage;
			_invinsible = true;
			_invinsibilityTime.Restart();
			SoundEffects::PLAYER_HITSOUND.play();
		}
	}
}



std::list<sf::FloatRect> Entity::GetHurtboxes() {
	return _hurtboxes;
}

std::list<sf::FloatRect> Entity::GetHitboxes() {
	return _hitboxes;
}

Entity::Type Entity::GetType() {
	return None;
}

int Entity::GetDamage() { 
	return _damage;
}

void Entity::Draw(sf::RenderWindow& window) {
	window.draw(_sprite);

	//Debug
	//Show hurtboxes
	if (_showCollisionBoxes) {
		sf::RectangleShape temp;
		temp.setFillColor(sf::Color::Blue);
		for (const auto& hurtbox : _hurtboxes) {
			temp.setPosition(sf::Vector2f(hurtbox.left, hurtbox.top));
			temp.setSize(sf::Vector2f(hurtbox.width, hurtbox.height));
			window.draw(temp);
		}

		temp.setFillColor(sf::Color::Yellow);
		for (const auto& hitbox : _hitboxes) {
			temp.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
			temp.setSize(sf::Vector2f(hitbox.width, hitbox.height));
			window.draw(temp);
		}
	}
}

void Entity::ToggleShowCollisionBoxes() {
	_showCollisionBoxes = abs(_showCollisionBoxes - 1);
}




// Default virtual functions, should be built upon in subclasses

void Entity::PauseTimers() {}

void Entity::ResumeTimers() {}

void Entity::HandleMovement() {}

void Entity::Update(float elapsedTime) {}
