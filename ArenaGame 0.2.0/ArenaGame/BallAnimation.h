#pragma once
#include "AnimationHandler.h"


class BallAnimation : public AnimationHandler {


public:
	BallAnimation(sf::Sprite* sprite);
	~BallAnimation();

	enum Animation { Idle, Charge };

	sf::Sprite* NewProjectile();

	//Function for getting the next frame of each animation
	void NextIdle();
	void NextCharging();
	void NextProjectile(sf::Sprite*, int&, Timer&);

private:

	sf::Texture& _spriteSheet = *FileCache::GetAnimationTexture("Textures/Full Sprite Sheet.png");

	sf::IntRect ProjectileFrame[2];
};