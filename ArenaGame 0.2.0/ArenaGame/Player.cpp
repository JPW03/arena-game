#include "Player.h"
#include "Main.h"
#include <SFML\System.hpp>
#include <iostream>


Player::Player(Arena* arena) :
	Entity(arena),
	_animation(PlayerAnimation(&_sprite)),
	_state(Grounded),
	_action(None),
	_moving(false), _dashing(false), _charging(false),
	_yVelocityMax(300.f),
	_gravity(800.f),
	_wallSlideVelocity(50.f),
	_jumpVelocity(-350.f),
	_walkingVelocity(130.f),
	_dashingVelocity(200.f),
	_comboValue(0),
	_jumpPressed(false), _dashPressed(false), _neutralPressed(false), _upAtkPressed(false), _sidePressed(false), _downAtkPressed(false) {

	//Set values of protected variables
	_direction = Right;
	_health = 100;
	_ignoresThinPlatforms = false;
	_damage = 0;

	_animation.SetFrame("Idle1");
	
	_sprite.setOrigin(_animation.spriteOrigin);

	_sprite.setPosition(arena->GetPlayerSpawn());

}

Player::~Player() {}





void Player::Update(float elapsedTime) {

	//Store new elapsed time
	_updateElapsed = elapsedTime;
	//_updateElapsed = 0.0166666666f;

	CheckInputs();

	//Check if the invinsibility frames have ran out
	if (_invinsible) {
		if (_invinsibilityTime.GetElapsedTime() >= 2.f) {
			_invinsible = false;
			_sprite.setScale(1, 1);
		}
		else {
			_animation.ShowInvinsibility();
		}
	}

	//Check for attack inputs
	switch (_action) {
	case (Jump):
	case (None):
		_boost = false;
		_comboValue = 0;
		CheckAttacks();
		break;
	}


	//Check current action, navigate to appropriate function:
	switch (_action) {

	/*case (Charging):			Not important for now
		_xVelocity = 0;
		_yVelocity = 0;

		_animation.NextCharged();
		
		if (_animation.GetFrame()->name == "")
		break;

	case (ChargedAtk):
		ChargedAtkUpdate();
		break;

	case (DashAtk):
		DashAtkUpdate();
		break;*/

	case (WallAtk):
		WallAtkUpdate();
		break;

	case (AirDown):
		AirDownUpdate();
		break;

	case (AirUp):
		AirUpUpdate();
		break;

	case (AirNeutral):
		AirNeutralUpdate();
		break;

	case (GrdSide):
		GrdSideUpdate();
		break;

	case (GrdUp):
		GrdUpUpdate();
		break;

	case (GrdNeutral):
		GrdNeutralUpdate();
		break;

	case (WallJump):
		WallJumpUpdate();
		break;

	default:	//No action ( = None)
		DefaultUpdate();
		break;
	}

	//Temporary movement system
	/*
	_yVelocity = 0.f, _xVelocity = 0.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		_yVelocity = -100.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		_yVelocity = 100.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		_xVelocity = -100.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		_xVelocity = 100.f;
	}*/


	//Remove a dash fade if not dashing
	if (!_dashing) {
		_animation.RemoveDashFade();
	}


	//Correct y velocity
	if (_yVelocity > _yVelocityMax) {
		_yVelocity = _yVelocityMax;
	}

	//Correct the direction the sprite is facing
	_animation.ChangeDirection(_direction);

	//Update sprite and handle collisions:
	HandleMovement();


	//Debug:
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
		std::cout << "(" << GetPosition().x << "," << GetPosition().y << ")" << ", Collisions: top " << _topColliding << ", bottom " << _bottomColliding << ", left " << _leftColliding << ", right " << _rightColliding << "\n";
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		std::cout << "_yVelocity = " << _yVelocity << ", _xVelocity = " << _xVelocity << ", _direction = " << _direction << ", _state = " << _state << ", _action = " << _action << ", _updateTime = " << _updateElapsed << "\n";
	}*/
}


void Player::DefaultUpdate() {
	DashUpdate();

	if (!_dashing) {
		HorizontalMovement(_walkingVelocity);
	}

	WallUpdate();

	ApplyGravity();

	//Jump
	JumpUpdate();

	//Animations
	_animation.hurtboxChange = false;
	if (_dashing) {
		_animation.NextDash(_topColliding);
		_animation.AddDashFade(GetPosition(), _direction);
	}
	else {
		if (_state == Wall) {
			_animation.NextWallSlide();
		}
		else {
			_state = Grounded;
			if (_topColliding) {
				if (!_moving) {
					_animation.NextIdle();
				}
				else {
					_animation.NextRunning();
				}
			}
			else if (_animation.GetFrame()->name != "Falling") {
				_state = Midair;
				_animation.SetFrame("Falling");
			}
			else {
				_state = Midair;	//Ensure the player's state is "midair" when in the falling frame
			}
		}
	}
}






void Player::Move(float deltaX, float deltaY) {
	Entity::Move(deltaX, deltaY);
	UpdateHurtboxes();
}

void Player::SetPosition(float x, float y) {
	Entity::SetPosition(x, y);
	UpdateHurtboxes();
}


void Player::HandleMovement() {

	UpdatePositionCache();

	//Update touchingPlatform cache (the platforms previously collided with)
	UpdatePlatformCache();

	UpdateHurtboxes();	//Update hurtboxes just before _currentPlatforms clears
	UpdateHitboxes();

	HandleCollision(_hurtboxes);

	//Move the player.
	Move(_xVelocity * _updateElapsed, _yVelocity * _updateElapsed);
}



void Player::UpdateHurtboxes() {
	_hurtboxes = _animation.GetFrame()->hurtboxes;

	Entity::UpdateHurtboxes();

	//Make sure the player hasn't clipped into a platform by changing direction or a change in the hurtboxes in the sprite.
	if ((_prevDirection != _direction || _animation.hurtboxChange) && _animation.GetPreviousFrame() != NULL) {

		//Show that the hurtboxes have been updated
		_prevDirection = _direction;
		_animation.hurtboxChange = false;

		sf::FloatRect currentOuterBounds = _animation.GetHurtboxOuterBounds();
		sf::FloatRect prevOuterBounds = _animation.GetHurtboxOuterBounds(_animation.GetPreviousFrame());
		Platform* temp = NULL;

		//Correct positions of outer bounds:
		switch (_direction) {
		case (Left):
			currentOuterBounds.top += GetPosition().y - _sprite.getOrigin().y;
			currentOuterBounds.left = (_sprite.getTextureRect().width - (currentOuterBounds.left + currentOuterBounds.width)) + GetPosition().x - (_sprite.getTextureRect().width - _sprite.getOrigin().x);
			prevOuterBounds.top += GetPosition().y - _sprite.getOrigin().y;
			prevOuterBounds.left = (_sprite.getTextureRect().width - (prevOuterBounds.left + prevOuterBounds.width)) + GetPosition().x - (_sprite.getTextureRect().width - _sprite.getOrigin().x);
			break;
		case (Right):
			currentOuterBounds.top += GetPosition().y - _sprite.getOrigin().y;
			currentOuterBounds.left += GetPosition().x - _sprite.getOrigin().x;
			prevOuterBounds.top += GetPosition().y - _sprite.getOrigin().y;
			prevOuterBounds.left += GetPosition().x - _sprite.getOrigin().x;
		}

		//Move the player back to the edge of the platforms being collided with
		//	Distance need to move = platform bound - hurtbox opposite bound
		Platform* prev = NULL;
		for (const auto& platform : _previousPlatforms) {
			
			//The hurtbox may change while in midair, in which case, the hurtbox needs to be checked with the previous 5 platforms to check for overlap

			if (platform.platformPtr != prev && platform.platformPtr->GetBoundingRect().intersects(currentOuterBounds) && (platform.platformPtr->GetType() != Platform::PlatformType::Thin || _topColliding)) {
				float d = 0;
				sf::FloatRect platRect = platform.platformPtr->GetBoundingRect();

				//Check which side of the platform to move it to (the nearest side)
				float leftDiff = (currentOuterBounds.left + currentOuterBounds.width) - platRect.left;
				float rightDiff = (platRect.left + platRect.width) - currentOuterBounds.left;
				float topDiff = (currentOuterBounds.top + currentOuterBounds.height) - platRect.top;
				float bottomDiff = (platRect.top + platRect.height) - currentOuterBounds.top;

				if (leftDiff > 0 && leftDiff < rightDiff && leftDiff < topDiff && leftDiff < bottomDiff) {
					d = -leftDiff;
					Move(d, 0);
				}
				else if (topDiff > 0 && topDiff < rightDiff && topDiff < leftDiff && topDiff < bottomDiff) {
					d = -topDiff;
					Move(0, d);
				}
				else if (rightDiff > 0 && rightDiff < leftDiff && rightDiff < topDiff && rightDiff < bottomDiff) {
					d = rightDiff;
					Move(d, 0);
				}
				else if (bottomDiff > 0 && bottomDiff < rightDiff && bottomDiff < leftDiff && bottomDiff < topDiff) {
					d = bottomDiff;
					Move(0, d);
				}

				prev = platform.platformPtr;
			}
		}
	}
}


void Player::UpdateHitboxes() {
	_hitboxes = _animation.GetFrame()->hitboxes;
	Entity::UpdateHitboxes();
}




void Player::CheckInputs() {
	//Future feature: gets the controls from the main object, allows for custom controls.
	
	if (sf::Keyboard::isKeyPressed(Main::GetKeybind(Main::KeyBinds::Left))) {
		_InputLeft = true;
	}
	else {
		_InputLeft = false;
	}
	if (sf::Keyboard::isKeyPressed(Main::GetKeybind(Main::KeyBinds::Right))) {
		_InputRight = true;
	}
	else {
		_InputRight = false;
	}
	if (sf::Keyboard::isKeyPressed(Main::GetKeybind(Main::KeyBinds::Jump))) {
		_InputJump = true;
	}
	else {
		_InputJump = false;
	}
	if (sf::Keyboard::isKeyPressed(Main::GetKeybind(Main::KeyBinds::Dash))) {
		_InputDash = true;
	}
	else {
		_InputDash = false;
	}
	if (sf::Keyboard::isKeyPressed(Main::GetKeybind(Main::KeyBinds::Neutral))) {
		_InputNeutral = true;
	}
	else {
		_InputNeutral = false;
	}
	if (sf::Keyboard::isKeyPressed(Main::GetKeybind(Main::KeyBinds::Side))) {
		_InputSide = true;
	}
	else {
		_InputSide = false;
	}
	if (sf::Keyboard::isKeyPressed(Main::GetKeybind(Main::KeyBinds::UpAtk))) {
		_InputUpAtk = true;
	}
	else {
		_InputUpAtk = false;
	}
	if (sf::Keyboard::isKeyPressed(Main::GetKeybind(Main::KeyBinds::DownAtk))) {
		_InputDownAtk = true;
	}
	else {
		_InputDownAtk = false;
	}
}



void Player::JumpUpdate() {
	//Initiate the jump
	if (_InputJump) {
		if (!_jumpPressed && _topColliding) {
			_action = Jump;
			_yVelocity = _jumpVelocity;
			_sprite.move(0, -1.f);	//In case player is stuck in platform.
			_state = Midair;
		}
		_jumpPressed = true;
	}
	else {
		//In case jump is ended prematurely
		if (_action == Jump) {
			_yVelocity = 20.f;
		}

		_action = None;
		_jumpPressed = false;
	}

	//When the player reaches peak, the jump has finished
	if (_yVelocity >= 0.f && _action == Jump) {
		_action = None;
	}
}

void Player::WallUpdate() {

	//Sliding on the right side of the platform
	if (_rightColliding) {
		//Player must move towards the wall to slide on it
		if (_InputLeft && !_topColliding) {
			_animation.ClearDashFade();
			_state = Wall;
			_direction = Left;
		}
		else {
			_state = Midair;
		}
	}

	//Sliding on the left side of the platform
	else if (_leftColliding) {
		if (_InputRight && !_topColliding) {
			_animation.ClearDashFade();
			_direction = Right;
			_state = Wall;
		}
		else {
			_state = Midair;
		}
	}

	if (_state == Wall) {
		_dashing = false;	//Hitting a wall stops the dash
		_action = None;		//Stops any action
		_yVelocity = _wallSlideVelocity;

		//Move slightly back into the wall to trigger another wall collision next update
		switch (_direction) {
		case (Left):
			_xVelocity = -1.f;
			break;
		case (Right):
			_xVelocity = 1.f;
			break;
		}

		//Check if the player has slid to the ground
		if (_topColliding) {
			_state = Grounded;
		}
		else if (!_rightColliding && !_leftColliding) {
			_state = Midair;
		}

		//If the state is still Wall and a jump is inputted, activate WallJump
		else if (_InputJump) {
			if (!_jumpPressed) {
				_action = WallJump;

				//Check if the player is dash wall jumping
				if (_InputDash) {
					_dashing = true;
					_dashPressed = true;
				}

				WallJumpUpdate();
			}
			_jumpPressed = true;
		}
		else {
			_jumpPressed = false;
		}
	}
}

void Player::ApplyGravity() {
	_yVelocity += _gravity * _updateElapsed;
}

void Player::HorizontalMovement(float velocity) {
	if (_InputLeft) {
		_direction = Left;
		_xVelocity = -velocity;
		_moving = true;
	}
	else if (_InputRight) {
		_direction = Right;
		_xVelocity = velocity;
		_moving = true;
	}
	else {
		_xVelocity = 0.f;
		_moving = false;
	}
}

void Player::DashUpdate() {
	//Check if the dash is active

	//Check if the dash input is new or not
	if (_InputDash && _topColliding) {
		if (!_dashPressed) {
			_dashing = true;
			_dashTime.Restart();
			_animation.ClearDashFade();
		}
		_dashPressed = true;
	}
	else if (_topColliding) {
		_dashPressed = false;
		_dashing = false;
	}


	if (_dashing) {
		//Check for a change in direction.
		if (_InputLeft) {
			_direction = Left;
		}
		if (_InputRight) {
			_direction = Right;
		}

		//Dash is time based instead of distance based (ends after an amount of time rather than distance travelled)

		//The dash will last for a certain number of seconds before stopping.
		if (_dashTime.GetElapsedTime() < 0.65f || !_topColliding) {
			if (_topColliding) {
				//Continuously move in the facing direction
				switch (_direction) {
				case (Left):
					_xVelocity = -_dashingVelocity;
					break;

				case (Right):
					_xVelocity = _dashingVelocity;
					break;
				}
			}
			else {
				HorizontalMovement(_dashingVelocity);
			}
		}
		else if (_topColliding) {
			_dashing = false;
		}
	}
}

void Player::WallJumpUpdate() {
	_animation.NextWallJumping(_dashing);

	//Initialise the jump
	if (_animation.GetFrame()->name == "WallJump1") {
		_yVelocity = -350.f;
	}
	else {
		ApplyGravity();
	}

	//If just wall jumping:
	if (!_dashing) {
		if (_animation.GetFrame()->order <= 1 && _animation.GetFrame()->animation == PlayerAnimation::Animation::WallJump) {
			switch (_direction) {
			case (Right):
				//when _leftColliding
				_xVelocity = -_walkingVelocity;
				break;

			case (Left):
				//when _rightColliding
				_xVelocity = _walkingVelocity;
				break;
			}
		}
		//If the animation is passed a certain point, allow the player to switch direction
		else {
			//Same code as HorizontalMovement(), but without the _direction changes
			if (_InputLeft) {
				_xVelocity = -_walkingVelocity;
				_moving = true;
			}
			else if (_InputRight) {
				_xVelocity = _walkingVelocity;
				_moving = true;
			}
			else {
				_xVelocity = 0.f;
				_moving = false;
			}

			//If the player lets go of jump, the wall jump ends.
			if (!_InputJump) {
				_yVelocity = 20.f;
				_jumpPressed = false;
				_animation.SetFrame("Falling");
			}
		}
	}

	//If wall jump dashing:
	if (_dashing) {
		if (_animation.GetFrame()->name == "DashMidair") {
			_animation.AddDashFade(GetPosition(), abs(_direction - 1));
		}
		else {
			_animation.AddDashFade(GetPosition(), _direction);
		}

		if (_animation.GetFrame()->order <= 1 && _animation.GetFrame()->animation == PlayerAnimation::Animation::WallJump) {
			switch (_direction) {
			case (Right):
				//when _leftColliding
				_xVelocity = -_dashingVelocity;
				break;

			case (Left):
				//when _rightColliding
				_xVelocity = _dashingVelocity;
				break;
			}
		}
		//If the animation is passed a certain point, allow the player to switch direction
		else {
			//Same code as HorizontalMovement(), but without the _direction changes
			if (_InputLeft) {
				_xVelocity = -_dashingVelocity;
				_moving = true;
			}
			else if (_InputRight) {
				_xVelocity = _dashingVelocity;
				_moving = true;
			}
			else {
				_xVelocity = 0.f;
				_moving = false;
			}

			//If the player lets go of jump, the wall jump ends.
			if (!_InputJump) {
				_yVelocity = 20.f;
				_jumpPressed = false;
				_animation.SetFrame("DashMidair");
			}
		}
	}

	
	//Once the animation reaches the final frame, end the action
	if (_animation.GetFrame()->name == "Falling" || _animation.GetFrame()->name == "DashMidair") {
		_direction = static_cast<Direction>(abs(_direction - 1));	//Switch direction
		_action = None;
		_state = Midair;
	}
}



//Attack functions

void Player::CheckAttacks() {

	//Charged attack - all attack buttons held down
	/*if (_InputNeutral && _InputSide && _InputUpAtk && _InputDownAtk) {
		if (!_neutralPressed && !_sidePressed && !_upAtkPressed && !_downAtkPressed) {
			if (_comboValue == 4) {	//Instantly into a charged attack if end of combo
				_action = ChargedAtk;
			}
			else if (_topColliding) {
				_action = Charging;
				_chargeTime.Restart();
			}
		}
	}*/

	if (_state == Grounded) {	//Grounded attacks
		/*if (_dashing) {
			if ((_InputNeutral && !_neutralPressed) || (_InputSide && !_sidePressed) && _comboValue < 2 ) {
				_action = DashAtk;
			}
		}*/
		if (_InputNeutral && !_neutralPressed && _comboValue < 2) {
			_action = GrdNeutral;
			_comboValue = 2;
		}
		else if (_InputSide && !_sidePressed && _comboValue < 1) {
			_action = GrdSide;
			_comboValue = 1;
		}
		else if ((_InputUpAtk && !_upAtkPressed) || (_InputDownAtk && !_downAtkPressed) && _comboValue < 3) {
			_action = GrdUp;
			_comboValue = 3;
		}
	}

	else if (_state == Wall) {
		if (_InputNeutral && !_neutralPressed && _comboValue < 1) {
			_action = WallAtk;
			_comboValue = 1;
		}
	}

	else if (!_dashing) {	//Midair attacks (can't attack while dashing in midair)
		if ((_InputNeutral && !_neutralPressed) || (_InputSide && !_sidePressed) && _comboValue < 4) {
			_action = AirNeutral;
			_comboValue = 4;
		}
		else if (_InputDownAtk && !_downAtkPressed && _comboValue < 4) {
			_action = AirDown;
			_comboValue = 4;
		}
		else if (_InputUpAtk && !_upAtkPressed && _comboValue < 3) {
			_action = AirUp;
			_comboValue = 3;
		}
	}


	//Update booleans to prevent the player from holding down to continuously attack
	if (_InputNeutral) {
		_neutralPressed = true;
	}
	else _neutralPressed = false;
	if (_InputSide) {
		_sidePressed = true;
	}
	else _sidePressed = false;
	if (_InputUpAtk) {
		_upAtkPressed = true;
	}
	else _upAtkPressed = false;
	if (_InputDownAtk) {
		_downAtkPressed = true;
	}
	else _downAtkPressed = false;
}


void Player::GrdNeutralUpdate() {
	_xVelocity = 0;
	_damage = 20;
	ApplyGravity();

	_animation.NextGrdNeutral();

	//In case of a higher combo value attack
	CheckAttacks();

	//When the animation finishes, change the action back to normal
	if (_animation.GetFrame()->name == "GrdNeutral5") {
		_action = None;
	}
}


void Player::GrdUpUpdate() {
	_xVelocity = 0;
	_damage = 30;
	ApplyGravity();	//Gravity is always applied


	_animation.NextGrdUp();

	CheckAttacks();

	//When the animation finishes, change the action back to normal
	if (_animation.GetFrame()->name == "GrdUp6") {
		_action = None;
	}
}


void Player::GrdSideUpdate() {
	//Can still walk
	HorizontalMovement(_walkingVelocity);

	//In case of walking off of a ledge
	ApplyGravity();

	_damage = 15;

	_animation.NextGrdSide(_xVelocity);

	CheckAttacks();

	if (_animation.GetFrame()->name == "GrdSide5" || !_topColliding) {
		_action = None;
	}
}


void Player::AirNeutralUpdate() {
	//Can still move
	HorizontalMovement(_walkingVelocity);

	ApplyGravity();

	_damage = 40;

	_animation.NextAirNeutral();

	CheckAttacks();

	if (_animation.GetFrame()->name == "AirNeutral7" || _topColliding) {
		_action = None;
	}
}


void Player::AirUpUpdate() {
	//Can still move
	HorizontalMovement(_walkingVelocity);

	ApplyGravity();

	_damage = 30;

	_animation.NextAirUp();

	CheckAttacks();

	//On the third frame apply a slight upward boost
	if (_animation.GetFrame()->name == "AirUp3" && !_boost) {
		_yVelocity = -170.f;
		_boost = true;
	}
	

	if (_animation.GetFrame()->name == "AirUp6" || _topColliding) {
		_action = None;
	}
}


void Player::AirDownUpdate() {
	//Can still move
	HorizontalMovement(_walkingVelocity / 2);

	ApplyGravity();

	_damage = 50;

	_animation.NextAirDown(_topColliding);

	CheckAttacks();

	//On the third frame thrust the player down 
	if (_animation.GetFrame()->name == "AirDown3" && !_boost) {
		_yVelocity = 400.f;
		_boost = true;
	}

	if (_animation.GetFrame()->name == "AirDownGetUp2") {
		_action = None;
	}
}


void Player::WallAtkUpdate() {
	//Still slides down
	_yVelocity = _wallSlideVelocity;
	_xVelocity = 0;

	_damage = 20.f;

	_animation.NextWallAtk();

	CheckAttacks();

	if (_animation.GetFrame()->name == "WallAtk5" || _topColliding) {
		_action = None;
	}
}


void Player::DashAtkUpdate() {
	//This will take some thinking about so I'm going to leave it, see if I get enough time for it
}





void Player::Draw(sf::RenderWindow& window) {

	//Draw fade (if dashing)
	std::vector<sf::Sprite*> fadeSprites = _animation.GetDashFade();
	for (auto& fade : fadeSprites) {
		window.draw(*fade);
	}

	//Draw extra leg sprite (if in the Grounded side attack)
	if (_animation.GetFrame()->animation == PlayerAnimation::GrdSide) {
		window.draw(*(_animation.GetLegSprite(_direction)));
	}

	Entity::Draw(window);
}



void Player::PauseTimers() {
	_dashTime.Pause();
	_chargeTime.Pause();
}

void Player::ResumeTimers() {
	_dashTime.Resume();
	_chargeTime.Resume();
}

Entity::Type Player::GetType() {
	return Entity::Type::Player;
}

int Player::GetComboValue() {
	return _comboValue;
}