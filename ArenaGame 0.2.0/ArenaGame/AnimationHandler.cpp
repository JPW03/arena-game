#include "AnimationHandler.h"
#include <iostream>



AnimationHandler::AnimationHandler(sf::Sprite* sprite, sf::Vector2f origin) :
	_sprite(sprite),
	spriteOrigin(origin),
	hurtboxChange(false) {}

AnimationHandler::~AnimationHandler() {}



AnimationHandler::Frame::Frame(std::string frameName, int frameOrder, int frameAnimation, sf::IntRect frameRect) :
	name(frameName), order(frameOrder), animation(frameAnimation), textureRect(frameRect) {}

AnimationHandler::Frame::~Frame() {}



void AnimationHandler::ChangeDirection(int direction) {
	//Show that the sprite and hurtboxes have changed
	hurtboxChange = true;

	switch (direction) {
		//direction values equivalent to Player::Direction enum values

	case (0): //Swapping to left
		_sprite->setScale(-abs(_sprite->getScale().x), _sprite->getScale().y);
		break;

	case (1): //Swapping to right
		_sprite->setScale(abs(_sprite->getScale().x), _sprite->getScale().y);
		break;

	default:
		std::cout << "Invalid direction (" << direction << ")\n";
		break;
	}
}


void AnimationHandler::ShowInvinsibility() {
	float elapsed = _invinsibilityTime.GetElapsedTime();

	//After 0.1 seconds the sprite returns to normal scale
	if (elapsed >= 0.1f) {
		_sprite->setScale(1, 1);
		_invinsibilityTime.Restart();
	}
	//After 0.05 seconds slightly inflate the sprite
	else if (elapsed >= 0.05f) {
		_sprite->setScale(1.1, 1.1);
	}

}


void AnimationHandler::SetFrame(std::string frameName) {
	//Restart frame timer
	_frameTime.Restart();

	//std::cout << "Updating player frame to: " << frameName << "\n";

	//Check if the given string matches the current frame
	if (_currentFrame != NULL) {
		if (_currentFrame->name == frameName) {
			return;
		}
	}

	//Search vector for a frame of matching name
	std::vector<Frame*>::iterator itr = _frames.begin();
	bool found = false;

	while (itr != _frames.end() && !found) {

		if ((*itr)->name == frameName) {
			found = true;
			if (_currentFrame != NULL) {

				//Check if the hurtboxes changed.
				bool match = false;
				for (const auto& hurtbox : _currentFrame->hurtboxes) {
					match = false;
					for (const auto& hurtboxNew : (*itr)->hurtboxes) {
						if (hurtbox == hurtboxNew) {
							match = true;
						}
					}
					if (!match) {
						break;
					}
				}
				if (!match) {
					hurtboxChange = true;
				}
				else {
					hurtboxChange = false;
				}


				_prevFrame = _currentFrame;
			}
			_currentFrame = *itr;

			//Update sprite position in case the change in rect height causes jumping
			_sprite->setTextureRect(_currentFrame->textureRect);
		}

		itr++;
	}

	//Throw an exception if a frame is not found
	if (!found)
		throw (std::runtime_error("Player: Frame of animation not found: " + frameName));
}


void AnimationHandler::SetFrame(int frameOrder) {
	//Restart frame timer	
	_frameTime.Restart();

	//std::cout << "Updating player frame to " << frameOrder << "\n";

	//Check if the given string matches the current frame
	if (_currentFrame != NULL) {
		if (_currentFrame->order == frameOrder) {
			return;
		}
	}

	//Search vector for a frame of matching name
	std::vector<Frame*>::iterator itr = _frames.begin();
	bool found = false;

	while (itr != _frames.end() && !found) {

		if ((*itr)->order == frameOrder && _currentFrame->animation == (*itr)->animation) {
			found = true;

			//Check if the hurtboxes changed.
			if (_currentFrame->hurtboxes != (*itr)->hurtboxes) {
				hurtboxChange = true;
			}
			else {
				hurtboxChange = false;
			}

			_prevFrame = _currentFrame;
			_currentFrame = *itr;

			//Update sprite position in case the change in rect height causes jumping
			_sprite->setTextureRect(_currentFrame->textureRect);
		}

		itr++;
	}


	//Throw an exception if a frame is not found
	if (!found)
		throw (std::runtime_error("Player: Unable to update to order " + frameOrder));
}





//Get methods

AnimationHandler::Frame* AnimationHandler::GetFrame() {
	return _currentFrame;
}

AnimationHandler::Frame* AnimationHandler::GetFrame(std::string frameName) {
	//Search vector for a frame of matching name
	std::vector<Frame*>::iterator itr = _frames.begin();
	bool found = false;

	while (itr != _frames.end()) {
		if ((*itr)->name == frameName) {
			found = true;
			return *itr;
		}
		itr++;
	}

	if (!found) {
		throw(std::runtime_error("GetFrame(std::string) : Frame '" + frameName + "' not found"));
	}
}

AnimationHandler::Frame* AnimationHandler::GetPreviousFrame() {
	return _prevFrame;
}

float AnimationHandler::GetFrameTime() {
	return _frameTime.GetElapsedTime();
}

//Returns the outer collision bounds of the specified frame.
sf::FloatRect AnimationHandler::GetHurtboxOuterBounds(Frame* frame) {
	sf::FloatRect outerBounds;

	std::list<sf::FloatRect>::iterator itr = frame->hurtboxes.begin();
	outerBounds = (*itr);	//If there's only 1 hurtbox, the outer bounds are equal to just the hurtbox.
	itr++;

	//If there are multiple hurtboxes, update the outer bounds according to the other hurtboxes.
	while (itr != frame->hurtboxes.end()) {
		//Update left
		if ((*itr).left < outerBounds.left) {
			outerBounds.width += (*itr).left - outerBounds.left;
			outerBounds.left = (*itr).left;
		}

		//Update right
		if ((*itr).left + (*itr).width > outerBounds.left + outerBounds.width) {
			outerBounds.width = ((*itr).left + (*itr).width) - outerBounds.left;
		}

		//Update top
		if ((*itr).top < outerBounds.top) {
			outerBounds.height += (*itr).top - outerBounds.top;
			outerBounds.top = (*itr).top;
		}

		//Update bottom
		if ((*itr).top + (*itr).height > outerBounds.top + outerBounds.height) {
			outerBounds.height = ((*itr).top + (*itr).height) - outerBounds.top;
		}

		itr++;
	}

	return outerBounds;
}

//Returns the outer collision bounds of the current frame.
sf::FloatRect AnimationHandler::GetHurtboxOuterBounds() {
	return GetHurtboxOuterBounds(_currentFrame);
}

//Returns the outer collision bounds of the specified frame.
sf::FloatRect AnimationHandler::GetHurtboxOuterBounds(std::string frameName) {
	return GetHurtboxOuterBounds(GetFrame(frameName));
}