#include "PlayerAnimation.h"
#include <iostream>


PlayerAnimation::~PlayerAnimation() {}

PlayerAnimation::PlayerAnimation(sf::Sprite* sprite) :
	AnimationHandler::AnimationHandler(sprite, sf::Vector2f(63.f, 63.f)),
	_newSpriteTemp(), _legSprite(),
	_currentLeg(0) {

	_sprite->setTexture(_spriteSheet);
	_legSprite = new sf::Sprite(_spriteSheet);
	_legSprite->setOrigin(spriteOrigin);

	//Construct each frame
	//The sprite sheet has 20 columns (every 20 frames, add 128 to the y)

	_frames.push_back(new Frame("Idle1", 1, Idle, sf::IntRect(0, 0, 128, 128)));
	_frames.push_back(new Frame("Idle2", 2, Idle, sf::IntRect(128, 0, 128, 128)));

	_frames.push_back(new Frame("Running1", 1, Running, sf::IntRect((128 * 2), 0, 128, 128)));	
	_frames.push_back(new Frame("Running2", 2, Running, sf::IntRect((128 * 3), 0, 128, 128)));
	_frames.push_back(new Frame("Running3", 3, Running, sf::IntRect((128 * 4), 0, 128, 128)));
	_frames.push_back(new Frame("Running4", 4, Running, sf::IntRect((128 * 5), 0, 128, 128)));
	_frames.push_back(new Frame("Running5", 5, Running, sf::IntRect((128 * 6), 0, 128, 128)));
	_frames.push_back(new Frame("Running6", 6, Running, sf::IntRect((128 * 7), 0, 128, 128)));
	_frames.push_back(new Frame("Running7", 7, Running, sf::IntRect((128 * 8), 0, 128, 128)));
	_frames.push_back(new Frame("Running8", 8, Running, sf::IntRect((128 * 9), 0, 128, 128)));
	_frames.push_back(new Frame("Running9", 9, Running, sf::IntRect((128 * 10), 0, 128, 128)));
	_frames.push_back(new Frame("Running10", 10, Running, sf::IntRect((128 * 11), 0, 128, 128)));
	_frames.push_back(new Frame("Running11", 11, Running, sf::IntRect((128 * 12), 0, 128, 128)));
	_frames.push_back(new Frame("Running12", 12, Running, sf::IntRect((128 * 13), 0, 128, 128)));
	_frames.push_back(new Frame("Running13", 13, Running, sf::IntRect((128 * 14), 0, 128, 128)));
	_frames.push_back(new Frame("Running14", 14, Running, sf::IntRect((128 * 15), 0, 128, 128)));
	_frames.push_back(new Frame("Running15", 15, Running, sf::IntRect((128 * 16), 0, 128, 128)));
	_frames.push_back(new Frame("Running16", 16, Running, sf::IntRect((128 * 17), 0, 128, 128)));

	_frames.push_back(new Frame("Jump1", 1, Jump, sf::IntRect((128 * 18), 0, 128, 128)));	//UNUSED
	_frames.push_back(new Frame("Jump2", 2, Jump, sf::IntRect((128 * 19), 128, 128, 128)));	//UNUSED

	_frames.push_back(new Frame("Falling", 3, Idle, sf::IntRect((128 * 0), 128, 128, 128)));

	_frames.push_back(new Frame("WallSlide", 1, WallSlide, sf::IntRect((128 * 1), 128, 128, 128)));

	_frames.push_back(new Frame("WallJump1", 1, WallJump, sf::IntRect((128 * 2), 128, 128, 128)));
	_frames.push_back(new Frame("WallJump2", 2, WallJump, sf::IntRect((128 * 3), 128, 128, 128)));
	_frames.push_back(new Frame("WallJump3", 3, WallJump, sf::IntRect((128 * 4), 128, 128, 128)));
	DashWallJumpFade[0] = sf::IntRect((128 * 5), 128, 128, 128);
	DashWallJumpFade[1] = sf::IntRect((128 * 6), 128, 128, 128);
	DashWallJumpFade[2] = sf::IntRect((128 * 7), 128, 128, 128);

	_frames.push_back(new Frame("Dash", 1, Dash, sf::IntRect((128 * 8), 128, 128, 128)));
	DashFade = sf::IntRect((128 * 9), 128, 128, 128);
	_frames.push_back(new Frame("DashMidair", 2, Dash, sf::IntRect((128 * 10), 128, 128, 128)));
	DashMidairFade = sf::IntRect((128 * 11), 128, 128, 128);
	//Fade texture rects are not Frame objects because they don't have a hitbox/hurtbox

	_frames.push_back(new Frame("GrdNeutral1", 1, GrdNeutral, sf::IntRect((128 * 12), 128, 128, 128)));
	_frames.push_back(new Frame("GrdNeutral2", 2, GrdNeutral, sf::IntRect((128 * 13), 128, 128, 128)));
	_frames.push_back(new Frame("GrdNeutral3", 3, GrdNeutral, sf::IntRect((128 * 14), 128, 128, 128)));
	_frames.push_back(new Frame("GrdNeutral4", 4, GrdNeutral, sf::IntRect((128 * 15), 128, 128, 128)));
	_frames.push_back(new Frame("GrdNeutral5", 5, GrdNeutral, sf::IntRect((128 * 16), 128, 128, 128)));
	
	_frames.push_back(new Frame("GrdSide1", 1, GrdSide, sf::IntRect((128 * 17), 128, 128, 128)));
	_frames.push_back(new Frame("GrdSide2", 2, GrdSide, sf::IntRect((128 * 18), 128, 128, 128)));
	_frames.push_back(new Frame("GrdSide3", 3, GrdSide, sf::IntRect((128 * 19), 128, 128, 128)));
	_frames.push_back(new Frame("GrdSide4", 4, GrdSide, sf::IntRect((128 * 0), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("GrdSide5", 5, GrdSide, sf::IntRect((128 * 1), (128 * 2), 128, 128)));

	_frames.push_back(new Frame("GrdUp1", 1, GrdUp, sf::IntRect((128 * 2), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("GrdUp2", 2, GrdUp, sf::IntRect((128 * 3), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("GrdUp3", 3, GrdUp, sf::IntRect((128 * 4), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("GrdUp4", 4, GrdUp, sf::IntRect((128 * 5), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("GrdUp5", 5, GrdUp, sf::IntRect((128 * 6), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("GrdUp6", 6, GrdUp, sf::IntRect((128 * 7), (128 * 2), 128, 128)));

	_frames.push_back(new Frame("AirNeutral1", 1, AirNeutral, sf::IntRect((128 * 8), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirNeutral2", 2, AirNeutral, sf::IntRect((128 * 9), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirNeutral3", 3, AirNeutral, sf::IntRect((128 * 10), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirNeutral4", 4, AirNeutral, sf::IntRect((128 * 11), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirNeutral5", 5, AirNeutral, sf::IntRect((128 * 12), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirNeutral6", 6, AirNeutral, sf::IntRect((128 * 13), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirNeutral7", 7, AirNeutral, sf::IntRect((128 * 14), (128 * 2), 128, 128)));

	_frames.push_back(new Frame("AirUp1", 1, AirUp, sf::IntRect((128 * 15), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirUp2", 2, AirUp, sf::IntRect((128 * 16), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirUp3", 3, AirUp, sf::IntRect((128 * 17), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirUp4", 4, AirUp, sf::IntRect((128 * 18), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirUp5", 5, AirUp, sf::IntRect((128 * 19), (128 * 2), 128, 128)));
	_frames.push_back(new Frame("AirUp6", 6, AirUp, sf::IntRect((128 * 0), (128 * 3), 128, 128)));

	_frames.push_back(new Frame("AirDown1", 1, AirDown, sf::IntRect((128 * 1), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("AirDown2", 2, AirDown, sf::IntRect((128 * 2), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("AirDown3", 3, AirDown, sf::IntRect((128 * 3), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("AirDown4", 4, AirDown, sf::IntRect((128 * 4), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("AirDownGetUp1", 5, AirDown, sf::IntRect((128 * 5), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("AirDownGetUp2", 6, AirDown, sf::IntRect((128 * 6), (128 * 3), 128, 128)));

	_frames.push_back(new Frame("DashAtk1", 1, DashAtk, sf::IntRect((128 * 7), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("DashAtk2", 2, DashAtk, sf::IntRect((128 * 8), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("DashAtk3", 3, DashAtk, sf::IntRect((128 * 9), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("DashAtk4", 4, DashAtk, sf::IntRect((128 * 10), (128 * 3), 128, 128)));
	DashAtkFade[0] = sf::IntRect((128 * 11), (128 * 3), 128, 128);
	DashAtkFade[1] = sf::IntRect((128 * 12), (128 * 3), 128, 128);

	_frames.push_back(new Frame("GrdCharged1", 1, GrdCharged, sf::IntRect((128 * 13), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("GrdCharged2", 2, GrdCharged, sf::IntRect((128 * 14), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("GrdCharged3", 3, GrdCharged, sf::IntRect((128 * 15), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("GrdCharged4", 4, GrdCharged, sf::IntRect((128 * 16), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("GrdCharged5", 5, GrdCharged, sf::IntRect((128 * 17), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("Charged1", 1, Charged, sf::IntRect((128 * 18), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("Charged2", 2, Charged, sf::IntRect((128 * 19), (128 * 3), 128, 128)));
	_frames.push_back(new Frame("Charged3", 3, Charged, sf::IntRect((128 * 0), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("Charged4", 4, Charged, sf::IntRect((128 * 1), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("Charged5", 5, Charged, sf::IntRect((128 * 2), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("Charged6", 6, Charged, sf::IntRect((128 * 3), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("Charged7", 7, Charged, sf::IntRect((128 * 4), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("GrdChargedEnd1", 6, GrdCharged, sf::IntRect((128 * 5), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("GrdChargedEnd2", 7, GrdCharged, sf::IntRect((128 * 6), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("GrdChargedEnd3", 8, GrdCharged, sf::IntRect((128 * 7), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("AirCharge1", 1, AirCharged, sf::IntRect((128 * 8), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("AirCharge2", 2, AirCharged, sf::IntRect((128 * 9), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("AirCharge3", 3, AirCharged, sf::IntRect((128 * 10), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("AirCharge4", 4, AirCharged, sf::IntRect((128 * 11), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("AirCharge5", 5, AirCharged, sf::IntRect((128 * 12), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("AirChargeEnd1", 6, AirCharged, sf::IntRect((128 * 13), (128 * 4), 128, 128)));

	_frames.push_back(new Frame("WallAtk1", 1, WallAtk, sf::IntRect((128 * 14), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("WallAtk2", 2, WallAtk, sf::IntRect((128 * 15), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("WallAtk3", 3, WallAtk, sf::IntRect((128 * 16), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("WallAtk4", 4, WallAtk, sf::IntRect((128 * 17), (128 * 4), 128, 128)));
	_frames.push_back(new Frame("WallAtk5", 5, WallAtk, sf::IntRect((128 * 18), (128 * 4), 128, 128)));

	RunningLegs[1] = sf::IntRect((128 * 19), (128 * 4), 128, 128);
	RunningLegs[2] = sf::IntRect((128 * 0), (128 * 5), 128, 128);
	RunningLegs[3] = sf::IntRect((128 * 1), (128 * 5), 128, 128);
	RunningLegs[4] = sf::IntRect((128 * 2), (128 * 5), 128, 128);
	RunningLegs[5] = sf::IntRect((128 * 3), (128 * 5), 128, 128);
	RunningLegs[6] = sf::IntRect((128 * 4), (128 * 5), 128, 128);
	RunningLegs[7] = sf::IntRect((128 * 5), (128 * 5), 128, 128);
	RunningLegs[8] = sf::IntRect((128 * 6), (128 * 5), 128, 128);
	RunningLegs[0] = sf::IntRect((128 * 16), (128 * 5), 128, 128);	//Idle legs




	sf::FloatRect defaultHurtbox = sf::FloatRect(53, 45, 26, 46);
	sf::FloatRect defaultMidairHurtbox = sf::FloatRect(53, 42, 26, 49);
	sf::FloatRect chargedAtkHurtbox = sf::FloatRect(50, 42, 23, 49);

	//Allocate hurtboxes and hitboxes
	for (const auto& frame : _frames) {

		switch (frame->animation) {

			//Attack animations
		case (GrdNeutral):
			frame->hurtboxes.push_back(defaultHurtbox);
			switch (frame->order) {
			case (3):
				frame->hitboxes.push_back(sf::FloatRect(86, 59, 24, 14));
				break;
			case (4):
				frame->hitboxes.push_back(sf::FloatRect(87, 49, 28, 10));
				break;
			case (5):
				frame->hitboxes.push_back(sf::FloatRect(86, 43, 26, 10));
				break;
			default:
				break;
			}
			break;

		case (GrdSide):
			frame->hurtboxes.push_back(defaultHurtbox);
			switch (frame->order) {
			case (2):
				frame->hitboxes.push_back(sf::FloatRect(93, 51, 29, 8));
				break;
			case (3):
				frame->hitboxes.push_back(sf::FloatRect(85, 65, 29, 10));
				break;
			case (4):
				frame->hitboxes.push_back(sf::FloatRect(78, 71, 25, 15));
				break;
			case (5):
				frame->hitboxes.push_back(sf::FloatRect(77, 72, 25, 15));
				break;
			default:
				break;
			}
			break;

		case (GrdUp):
			switch (frame->order) {
			case (2):
				frame->hurtboxes.push_back(sf::FloatRect(53, 41, 26, 50));
				frame->hitboxes.push_back(sf::FloatRect(84, 34, 21, 36));
				break;
			case (3):
				frame->hurtboxes.push_back(sf::FloatRect(53, 39, 26, 52));
				frame->hitboxes.push_back(sf::FloatRect(65, 5, 21, 28));
				frame->hitboxes.push_back(sf::FloatRect(88, 19, 13, 13));
				break;
			case (4):
				frame->hurtboxes.push_back(sf::FloatRect(53, 41, 26, 50));
				frame->hitboxes.push_back(sf::FloatRect(60, 7, 29, 31));
				break;
			case (5):
				frame->hurtboxes.push_back(sf::FloatRect(53, 41, 26, 50));
				frame->hitboxes.push_back(sf::FloatRect(25, 36, 27, 11));
				break;
			case (6):
				frame->hurtboxes.push_back(defaultHurtbox);
				frame->hitboxes.push_back(sf::FloatRect(30, 59, 25, 16));
				break;
			default:
				frame->hurtboxes.push_back(defaultHurtbox);
				break;
			}
			break;

		case (AirNeutral):
			switch (frame->order) {
			case (4):
				frame->hurtboxes.push_back(defaultMidairHurtbox);
				frame->hitboxes.push_back(sf::FloatRect(58, 12, 19, 26));
				break;
			case (5):
				frame->hurtboxes.push_back(defaultHurtbox);
				frame->hitboxes.push_back(sf::FloatRect(80, 16, 33, 52));
				break;
			case (6):
				frame->hurtboxes.push_back(defaultHurtbox);
				frame->hitboxes.push_back(sf::FloatRect(84, 63, 26, 26));
				break;
			case (7):
				frame->hurtboxes.push_back(defaultHurtbox);
				frame->hitboxes.push_back(sf::FloatRect(83, 74, 24, 16));
				break;
			default:
				frame->hurtboxes.push_back(defaultMidairHurtbox);
				break;
			}
			break;

		case (AirDown):
			switch (frame->order) {
			case (3):
				frame->hurtboxes.push_back(sf::FloatRect(53, 42, 26, 43));
				frame->hitboxes.push_back(sf::FloatRect(71, 59, 14, 38));
				break;
			case (4):
				frame->hurtboxes.push_back(sf::FloatRect(53, 42, 26, 43));
				frame->hitboxes.push_back(sf::FloatRect(69, 52, 20, 50));
				break;
			case (5):
				frame->hurtboxes.push_back(sf::FloatRect(53, 46, 26, 39));
				break;
			case (6):
				frame->hurtboxes.push_back(defaultHurtbox);
				break;
			default:
				frame->hurtboxes.push_back(sf::FloatRect(53, 42, 26, 45));
				break;
			}
			break;

		case (AirUp):
			frame->hurtboxes.push_back(defaultMidairHurtbox);
			switch (frame->order) {
			case (2):
				frame->hitboxes.push_back(sf::FloatRect(84, 34, 21, 36));
				break;
			case (3):
				frame->hurtboxes.clear();
				frame->hurtboxes.push_back(sf::FloatRect(53, 38, 26, 53));
				frame->hitboxes.push_back(sf::FloatRect(65, 5, 21, 28));
				frame->hitboxes.push_back(sf::FloatRect(88, 19, 13, 13));
				break;
			case (4):
				frame->hitboxes.push_back(sf::FloatRect(60, 7, 29, 31));
				break;
			case (5):
				frame->hitboxes.push_back(sf::FloatRect(25, 36, 27, 11));
				break;
			case (6):
				frame->hitboxes.push_back(sf::FloatRect(30, 59, 25, 16));
				break;
			default:
				break;
			}
			break;

		case (DashAtk):
			frame->hurtboxes.push_back(sf::FloatRect(50, 52, 27, 39));
			if (frame->order > 1) {
				frame->hitboxes.push_back(sf::FloatRect(79, 59, 14, 26));
				frame->hitboxes.push_back(sf::FloatRect(93, 62, 14, 20));
			}
			break;

		case (GrdCharged):
			switch (frame->order) {
			case (5 || 6):
				frame->hurtboxes.push_back(chargedAtkHurtbox);
				break;
			default:
				frame->hurtboxes.push_back(defaultHurtbox);
				break;
			}
			break;
		
		case (AirCharged):
			switch (frame->order) {
			case (5):
				frame->hurtboxes.push_back(chargedAtkHurtbox);
				break;
			case (6):
				frame->hurtboxes.push_back(sf::FloatRect(53, 42, 23, 49));
				break;
			default:
				frame->hurtboxes.push_back(defaultMidairHurtbox);
				break;
			}
			break;

		case (Charged):
			frame->hurtboxes.push_back(chargedAtkHurtbox);
			switch (frame->order) {
			case (1):
				frame->hitboxes.push_back(sf::FloatRect(76, 43, 33, 15));
				break;
			case (2):
				frame->hitboxes.push_back(sf::FloatRect(55, 47, 54, 13));
				break;
			case (3):
				frame->hitboxes.push_back(sf::FloatRect(20, 46, 89, 17));
				break;
			case (4):
				frame->hitboxes.push_back(sf::FloatRect(20, 44, 89, 19));
				break;
			default:
				frame->hitboxes.push_back(sf::FloatRect(20, 41, 89, 22));
				break;
			}
			break;

		case (WallAtk):
			frame->hurtboxes.push_back(defaultMidairHurtbox);
			switch (frame->order) {
			case (2):
				frame->hitboxes.push_back(sf::FloatRect(39, 71, 17, 23));
				break;
			case (3):
				frame->hitboxes.push_back(sf::FloatRect(17, 56, 29, 26));
				break;
			case (4):
				frame->hitboxes.push_back(sf::FloatRect(29, 29, 27, 24));
				frame->hitboxes.push_back(sf::FloatRect(19, 40, 9, 15));
				break;
			case (5):
				frame->hitboxes.push_back(sf::FloatRect(30, 26, 28, 19));
				break;
			default:
				break;
			}
			break;





			//Movement animations
		default:
			if (frame->name == "Dash") {
				frame->hurtboxes.push_back(sf::FloatRect(50, 50, 27, 41));
				//frame->hurtboxes.push_back(sf::FloatRect(62, 50, 15, 18));
				//frame->hurtboxes.push_back(sf::FloatRect(45, 56, 17, 35));
			}
			else if (frame->name == "DashMidair") {
				frame->hurtboxes.push_back(sf::FloatRect(54, 43, 23, 48));
			}
			else if (frame->name == "Falling" || frame->name == "WallSlide") {
				frame->hurtboxes.push_back(defaultMidairHurtbox);
			}
			else {
				frame->hurtboxes.push_back(defaultHurtbox);
			}
			break;


		}
	}
}




//Animation frame switching functions

//Cycle between grounded idle animations
void PlayerAnimation::NextIdle() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation != Idle || _currentFrame->name == "Falling") {
			SetFrame("Idle1");
		}
		else if (_frameTime.GetElapsedTime() >= 0.4) {
			switch (_currentFrame->order) {
			case (1):
				SetFrame("Idle2");
				break;
			case (2):
				SetFrame("Idle1");
				break;
			}
		}
	}
}


void PlayerAnimation::NextRunning() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		//Set frame to next running frame in order
		if (_currentFrame->animation == Running) {
			//Maybe use a long switch statement instead? Faster?

			if (_currentFrame->order == 16) {
				SetFrame(1);
			}
			else if (_currentFrame->order < 16) {
				SetFrame(_currentFrame->order + 1);
			}
		}

		//If the animation is not already running, start the running animation
		else {
			SetFrame("Running1");
		}
	}
}


void PlayerAnimation::NextJumping() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == Jump) {
			switch (_currentFrame->order) {
			case (1):
				SetFrame("Jump2");
				break;
			case (2):
				SetFrame("Falling");
				break;
			}
		}
		else {
			SetFrame("Jump1");
		}
	}
}//UNUSED


void PlayerAnimation::NextWallSlide() {
	if (_frameTime.GetElapsedTime() >= 0.1 && _currentFrame->name != "WallSlide") {
		SetFrame("WallSlide");
	}
}


void PlayerAnimation::NextWallJumping(bool dashing) {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == WallJump) {
			if (_currentFrame->order < 3) {
				SetFrame(_currentFrame->order + 1);
			}
			else if (_currentFrame->order == 3) {
				if (dashing) {
					SetFrame("DashMidair");
				}
				else {
					SetFrame("Falling");
				}
			}
		}
		else {
			SetFrame("WallJump1");
		}
	}
}


void PlayerAnimation::NextDash(bool topColliding) {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (topColliding && _currentFrame->name != "Dash") {
			SetFrame("Dash");
		}
		else if (!topColliding && _currentFrame->name != "DashMidair") {
			SetFrame("DashMidair");
		}
		if (_currentFrame->animation != Dash)
			_fadeTime.Restart();
	}
}


void PlayerAnimation::NextGrdNeutral() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == GrdNeutral) {
			switch (_currentFrame->order) {
			case (5):
				SetFrame("Idle1");
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else
			SetFrame("GrdNeutral1");
	}
}


void PlayerAnimation::NextGrdSide(float xVelocity) {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == GrdSide) {
			switch (_currentFrame->order) {

			case (5):	//End of the animation
				if (xVelocity != 0.f) {	//Resume running animation if moving
					switch (_currentLeg) {
					case (2):
						SetFrame("Running2");
						break;
					case (3):
						SetFrame("Running3");
						break;
					case (4):
						SetFrame("Running4");
						break;
					case (5):
						SetFrame("Running5");
						break;
					case (6):
						SetFrame("Running6");
						break;
					case (7):
						SetFrame("Running7");
						break;
					case (8):
						SetFrame("Running8");
						break;
					default:	//0 or 1
						SetFrame("Running1");
						break;
					}
				}
				else {	//Resume the idle animation if not moving
					SetFrame("Idle1");
				}
				return;		//To stop the function from updating the _currentLegs again outside the switch function
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}

			//Update the leg sprite
			if (xVelocity != 0.f) {	//If moving
				if (_currentLeg == 8) {
					_currentLeg = 1;
				}
				else {
					_currentLeg++;
				}
			}
			else {
				_currentLeg = 0;
			}
		}
		else {
			//Figure out which leg sprite is needed based on the running animation frame
			if (xVelocity != 0.f) {
				if (_currentFrame->animation == Running) {
					if (_currentFrame->order > 8) {
						_currentLeg = _currentFrame->order - 8;
					}
					else {
						_currentLeg = _currentFrame->order;
					}
				}
				else {
					_currentLeg = 1;
				}
			}
			else {
				_currentLeg = 0;
			}

			//Start the animation
			SetFrame("GrdSide1");
		}
	}
}


void PlayerAnimation::NextGrdUp() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == GrdUp) {
			switch (_currentFrame->order) {
			case (6):
				SetFrame("Idle1");
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else
			SetFrame("GrdUp1");
	}
}


void PlayerAnimation::NextAirNeutral() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == AirNeutral) {
			switch (_currentFrame->order) {
			case (7):
				SetFrame("Falling");
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else
			SetFrame("AirNeutral1");
	}
}


void PlayerAnimation::NextAirUp() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == AirUp) {
			switch (_currentFrame->order) {
			case (6):
				SetFrame("Falling");
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else
			SetFrame("AirUp1");
	}
}


void PlayerAnimation::NextAirDown(bool topColliding) {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == AirDown) {
			if (!topColliding) {	//Striking down animation
				switch (_currentFrame->order) {
				case (5):
					SetFrame(4);
					break;
				case (6):
					SetFrame(4);
					break;
				case (4):
					break;
				default:
					SetFrame(_currentFrame->order + 1);
					break;
				}
			}
			else {	//Get up animation
				switch (_currentFrame->order) {
				case (5):
					SetFrame(6);
					break;
				case (6):
					SetFrame("Idle1");
					break;
				default:
					SetFrame(5);
					break;
				}
			}
		}
		else
			SetFrame("AirDown1");
	}
}


void PlayerAnimation::NextDashAtk() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == DashAtk) {
			switch (_currentFrame->order) {
			case (4):
				break;	//Stays on DashAtk4 until the dash ends
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else
			SetFrame("DashAtk1");
	}
}


void PlayerAnimation::NextWallAtk() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == WallAtk) {
			switch (_currentFrame->order) {
			case (5):
				SetFrame("WallSlide");
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else
			SetFrame("WallAtk1");
	}
}


void PlayerAnimation::NextGrdCharged() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == GrdCharged) {
			switch (_currentFrame->order) {
			case (5):
				NextCharged();
				break;
			case (8):
				SetFrame("Idle1");
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else if (_currentFrame->animation == Charged) {
			switch (_currentFrame->order) {
			case (7):
				SetFrame("GrdChargeEnd1");
				break;
			default:
				NextCharged();
				break;
			}
		}
		else
			SetFrame("GrdCharge1");
	}
}


void PlayerAnimation::NextAirCharged() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == AirCharged) {
			switch (_currentFrame->order) {
			case (5):
				NextCharged();
				break;
			case (6):
				SetFrame("Falling");
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else if (_currentFrame->animation == Charged) {
			switch (_currentFrame->order) {
			case (7):
				SetFrame("AirChargeEnd1");
				break;
			default:
				NextCharged();
				break;
			}
		}
		else
			SetFrame("GrdCharge1");
	}
}


void PlayerAnimation::NextCharged() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == Charged) {
			if (_currentFrame->order < 7)
				SetFrame(_currentFrame->order + 1);
		}
		else
			SetFrame("Charged1");
	}
}




//Dash Fade Functions

void PlayerAnimation::ClearDashFade() {
	_fadeSprites.clear();
	_fadeCounter = 0;
	for (int p = 0; p < 4; p++)
		_fadePosCache[p] = sf::Vector2f();
}


void PlayerAnimation::AddDashFade(sf::Vector2f position, int direction) {
	//Adds the current position to the front of the array and moves other positions back 1 space.
	for (int i = 4; i > 0; i--) {
		_fadePosCache[i] = _fadePosCache[i - 1];
	}
	_fadePosCache[0] = position;

	if (_fadeTime.GetElapsedTime() >= 0.1) {
		if (_fadeCounter >= 0) {

			//Get the correct texture rect to match the main sprite.
			sf::IntRect rect;
			switch (_currentFrame->animation) {
			case (Dash):
				if (_currentFrame->name == "Dash") {
					rect = DashFade;
				}
				else if (_currentFrame->name == "DashMidair") {
					rect = DashMidairFade;
				}
				break;

			case (WallJump):
				rect = DashWallJumpFade[_currentFrame->order - 1];
				break;
				
			case (DashAtk):
				switch (_currentFrame->order) {
				case (1):
					rect = DashAtkFade[0];
					break;
				default:
					rect = DashAtkFade[1];
					break;
				}
			}

			_newSpriteTemp = new sf::Sprite(_spriteSheet, rect);

			//Check the new sprite's direction
			switch (direction) {
				//direction values equivalent to Player::Direction enum values

			case (0): //Swapping to left
				_newSpriteTemp->setScale(-1, 1);
				break;

			case (1): //Swapping to right
				_newSpriteTemp->setScale(1, 1);
				break;

			default:
				std::cout << "Invalid direction (" << direction << ")\n";
				break;
			}

			_newSpriteTemp->setOrigin(spriteOrigin);
			_newSpriteTemp->setPosition(_fadePosCache[0]);

			_fadeSprites.push_back(_newSpriteTemp);
			
			if (_fadeSprites.size() == 4) {
				_fadeSprites.erase(_fadeSprites.begin());
			}
		}

		_fadeCounter++;
		_fadeTime.Restart();
	}

	
}

void PlayerAnimation::RemoveDashFade() {
	//Removes the sprite in the first order of the vector, if the vector isn't empty
	if (_fadeSprites.size() != 0 && _fadeTime.GetElapsedTime() >= 0.05) {
		_fadeSprites.erase(_fadeSprites.begin());
		_fadeTime.Restart();
	}
}


std::vector<sf::Sprite*> PlayerAnimation::GetDashFade() {
	return _fadeSprites;
}




//Extra leg sprite functions for GrdSide animation

sf::Sprite* PlayerAnimation::GetLegSprite(int direction) {
	_legSprite->setTextureRect(RunningLegs[_currentLeg]);
	_legSprite->setPosition(_sprite->getPosition());
	
	//Change the sprite's direction
	switch (direction) {
	case (0):	//Left
		_legSprite->setScale(_sprite->getScale());
		break;
	case (1):	//Right
		_legSprite->setScale(_sprite->getScale());
		break;
	}

	return _legSprite;
}