#pragma once
#include <SFML\Graphics.hpp>
#include <list>
#include "AnimationHandler.h"

//Forward declaration to prevent error
//(Player already includes "PlayerAnimation.h", so including "Player.h" would create circular dependency)
class Player;

//Consider moving the animation handling to here?
//This includes:
//	- std::map for name and intRect
//	- SetFrame function
//	- Pointer to sprite

//Alternative plan:
//	- Struct for each frame containing name, texture rects, hitbox rects, hurtbox rects (if any)
//	- List for structs


class PlayerAnimation : public AnimationHandler {

public:
	
	PlayerAnimation(sf::Sprite* sprite);
	~PlayerAnimation();

	enum Animation { Idle, Running, Jump, WallSlide, WallJump, Dash, GrdNeutral, GrdSide, GrdUp, AirNeutral, AirUp, AirDown, DashAtk, GrdCharged, AirCharged, Charged, WallAtk };
	//Grd = Grounded attack, Air = Aerial attack, Atk = Attack
	//Charged = Common frames for both grd and air variants

	//Functions for each animation. They change the current frame based on which animation is occuring.
	void NextIdle();
	void NextRunning();
	void NextJumping();
	void NextWallSlide();
	void NextWallJumping(bool dashing);
	void NextDash(bool topColliding);
	void NextGrdNeutral();
	void NextGrdSide(float xVelocity);
	void NextGrdUp();
	void NextAirNeutral();
	void NextAirDown(bool topColliding);
	void NextAirUp();
	void NextDashAtk();
	void NextGrdCharged();
	void NextAirCharged();
	void NextCharged();
	void NextWallAtk();
	
	void AddDashFade(sf::Vector2f position, int direction);
	void RemoveDashFade();
	void ClearDashFade();

	std::vector<sf::Sprite*> GetDashFade();
	sf::Sprite* GetLegSprite(int direction);	//For the ground side attack animation

private:

	sf::Texture& _spriteSheet = *FileCache::GetAnimationTexture("Textures/Full Sprite Sheet.png");

	std::vector<sf::Sprite*> _fadeSprites;
	sf::Sprite* _newSpriteTemp;
	int _fadeCounter;
	sf::Vector2f _fadePosCache[30];
	Timer _fadeTime;

	sf::IntRect DashFade;
	sf::IntRect DashMidairFade;
	sf::IntRect DashWallJumpFade[5];
	sf::IntRect DashAtkFade[2];

	//Legs from running animation are separate to allow for the side grounded attack to be synced with running animation
	sf::IntRect RunningLegs[9];
	int _currentLeg;
	sf::Sprite* _legSprite;
};