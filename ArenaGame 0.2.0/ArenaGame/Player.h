#pragma once
#include "Arena.h"
#include "FileCache.h"
#include "PlayerAnimation.h"
#include "Timer.h"
#include "Entity.h"


class Player : public Entity {

public:

	Player(Arena* arena);
	~Player();

	//All the possible states the player can be in when used in combination
	enum State { Grounded, Midair, Wall };
	enum Action { None, WallJump, Jump, GrdNeutral, GrdSide, GrdUp, AirNeutral, AirUp, AirDown, DashAtk, WallAtk, ChargedAtk, Charging };

	Entity::Type GetType();
	int GetComboValue();

	void Update(float elapsedTime);		//Update state if needed, then direct to suitable state.
	void Draw(sf::RenderWindow& window);

	void Move(float deltaX, float deltaY);
	void SetPosition(float x, float y);

	void PauseTimers();
	void ResumeTimers();

private:

	//Player movement
	State _state;
	Action _action;
	bool _moving;	//True if the player was moving in the last update.
	bool _dashing;
	bool _boost;	//For tracking when a boost in the middle of an action has occured
	bool _charging;

	//Combo priority value
	//Any attacks of higher priority value can instantly be done out of an attack
	int _comboValue;	//Default is 0 (no attack)

	//Constants
	const float _yVelocityMax;
	const float _wallSlideVelocity;
	const float _gravity;		//pixels per second^2
	const float _jumpVelocity;
	const float _walkingVelocity;
	const float _dashingVelocity;

	//Timers
	Timer _dashTime;		//Tracks the time the dash has lasted
	Timer _chargeTime;		//Tracks how long since the charged attack started charging

	//Dash Fade
	int _dashFadeCount;		//Tracks the number of fade sprites generated since the dash started.

	//Button booleans
	bool _InputLeft;
	bool _InputRight;
	bool _InputJump;
	bool _InputDash;
	bool _InputNeutral;
	bool _InputSide;
	bool _InputUpAtk;
	bool _InputDownAtk;
	void CheckInputs();
	void CheckAttacks();

	//Bools for making sure holding down a key doesn't repeat an action
	//True if key was pressed in previous update
	bool _jumpPressed;
	bool _dashPressed;
	bool _neutralPressed;
	bool _sidePressed;
	bool _upAtkPressed;
	bool _downAtkPressed;

	PlayerAnimation _animation;

	//Collision functions
	void HandleMovement();
	void UpdateHurtboxes();
	void UpdateHitboxes();

	//Updates for each action and/or state.
	//These handle inputs and velocities
	void WallUpdate();
	void ApplyGravity();
	void HorizontalMovement(float velocity);
	void JumpUpdate();
	void DashUpdate();
	void WallJumpUpdate();

	void GrdNeutralUpdate();
	void GrdSideUpdate();
	void GrdUpUpdate();
	void AirNeutralUpdate();
	void AirUpUpdate();
	void AirDownUpdate();
	void DashAtkUpdate();
	void ChargedAtkUpdate();
	void WallAtkUpdate();
	void DefaultUpdate();
};