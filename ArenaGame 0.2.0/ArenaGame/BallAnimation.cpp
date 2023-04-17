#include "BallAnimation.h"
#include <iostream>


BallAnimation::~BallAnimation() {}

BallAnimation::BallAnimation(sf::Sprite* sprite) :
	AnimationHandler::AnimationHandler(sprite, sf::Vector2f(63.f, 63.f)) {

	_sprite->setTexture(_spriteSheet);

	//Construct frame objects
	_frames.push_back(new Frame("Idle", 1, Idle, sf::IntRect((128 * 7), (128 * 5), 128, 128)));
	_frames.push_back(new Frame("Charge1", 1, Charge, sf::IntRect((128 * 8), (128 * 5), 128, 128)));
	_frames.push_back(new Frame("Charge2", 2, Charge, sf::IntRect((128 * 9), (128 * 5), 128, 128)));
	_frames.push_back(new Frame("Charge3", 3, Charge, sf::IntRect((128 * 10), (128 * 5), 128, 128)));
	_frames.push_back(new Frame("Charge4", 4, Charge, sf::IntRect((128 * 11), (128 * 5), 128, 128)));
	_frames.push_back(new Frame("Charge5", 5, Charge, sf::IntRect((128 * 12), (128 * 5), 128, 128)));
	_frames.push_back(new Frame("Charge6", 6, Charge, sf::IntRect((128 * 13), (128 * 5), 128, 128)));

	//Projectile sprite rects: (offset 52 pixels from top right corner of frame)
	ProjectileFrame[0] = sf::IntRect((128 * 14) + 52, (128 * 5) + 52, 24, 24);
	ProjectileFrame[1] = sf::IntRect((128 * 15) + 52, (128 * 5) + 52, 24, 24);


	//Allocate collision boxes
	for (auto& frame : _frames) {
		frame->hurtboxes.push_back(sf::FloatRect(38, 38, 52, 52));
	}
}



//Animation functions

void BallAnimation::NextIdle() {
	if (_frameTime.GetElapsedTime() >= 0.1f) {
		if (_currentFrame->animation == Idle) {
			return;
		}
		else {
			SetFrame("Idle");
		}
	}
}


void BallAnimation::NextCharging() {
	if (_frameTime.GetElapsedTime() >= 0.1) {
		if (_currentFrame->animation == Charge) {
			switch (_currentFrame->order) {
			case (6):
				SetFrame("Idle");
				break;
			default:
				SetFrame(_currentFrame->order + 1);
				break;
			}
		}
		else
			SetFrame("Charge1");
	}
}



//Projectile functions

sf::Sprite* BallAnimation::NewProjectile() {
	return new sf::Sprite(_spriteSheet, ProjectileFrame[0]);
}


void BallAnimation::NextProjectile(sf::Sprite* sprite, int& frame, Timer& timer) {
	if (timer.GetElapsedTime() >= 0.2f) {
		if (frame == 0) {
			sprite->setTextureRect(ProjectileFrame[1]);
		}
		else {
			sprite->setTextureRect(ProjectileFrame[0]);
		}
		frame = abs(frame - 1);
		timer.Restart();
	}
}