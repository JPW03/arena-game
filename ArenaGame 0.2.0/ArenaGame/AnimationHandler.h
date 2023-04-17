#pragma once
#include <SFML\Graphics.hpp>
#include <list>
#include "Timer.h"
#include "FileCache.h"


//Consider moving the animation handling to here?
//This includes:
//	- std::map for name and intRect
//	- SetFrame function
//	- Pointer to sprite

//Alternative plan:
//	- Struct for each frame containing name, texture rects, hitbox rects, hurtbox rects (if any)
//	- List for structs
//		^ went with this


class AnimationHandler {

public:
	AnimationHandler(sf::Sprite* sprite, sf::Vector2f origin);
	~AnimationHandler();


	struct Frame {
		Frame(std::string, int, int, sf::IntRect);
		~Frame();

		std::string name;
		int order;	//Counts up from 1 (the first frame of the animation)
		int animation;
		sf::IntRect textureRect;
		std::list<sf::FloatRect> hitboxes;
		std::list<sf::FloatRect> hurtboxes;
	};


	void SetFrame(std::string);
	void SetFrame(int frameOrder);	//For switching to a frame of different order within the same animation
	void ChangeDirection(int direction);	//For swapping between left and right animations
	void ShowInvinsibility();	//For slightly inflating and deflating the sprite to display invinsibility frames


	Frame* GetFrame();
	Frame* GetFrame(std::string);
	Frame* GetPreviousFrame();
	float GetFrameTime();
	sf::FloatRect GetHurtboxOuterBounds();							//Returns the outer collision bounds of the current frame, relative to the top left corner of the sprite.
	sf::FloatRect GetHurtboxOuterBounds(std::string frameName);		//Returns the outer collision bounds of a specified frame, relative to the top left corner of the sprite.
	sf::FloatRect GetHurtboxOuterBounds(Frame* frame);

	const sf::Vector2f spriteOrigin;	//The centre of the torso, coords relative to the top left of texture rect
	bool hurtboxChange;		//True if the frame has changed in the previous update.

protected:

	sf::Sprite* _sprite;
	
	Timer _frameTime;		//Tracks time since last animation frame.
	Timer _invinsibilityTime;

	std::vector<Frame*> _frames;
	Frame* _currentFrame;
	Frame* _prevFrame;
};