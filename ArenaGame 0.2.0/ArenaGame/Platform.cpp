#include "Platform.h"
#include <SFML\Graphics.hpp>
#include <algorithm>


//Create the rectangle shape representing the platform. This can be swapped out later for Sprites instead.
Platform::Platform(float midX, float midY, int width, int height, PlatformType type) {

	_midX = static_cast<float>(midX);
	_midY = static_cast<float>(midY);
	_width = static_cast<float>(width);
	_height = static_cast<float>(height);

	//Setup shape
	_shape.setSize(sf::Vector2f(_width, _height));
	//The origin of the shape will be its centre:
	_shape.setOrigin(_width / 2, _height / 2);
	//Set position based on new origin:
	_shape.setPosition(_midX, _midY);

	switch (type) {

	case (Solid):
		_shape.setTexture(FileCache::GetArenaTexture("Textures/cstle_tileset 1.png"));
		_shape.setTextureRect(sf::IntRect(14, 12, 31, 20));
		_shape.setPosition(_midX, _midY);
		break;


	case (CastleTest):
		CreateCastleTestPlatform();
		break;

	case (Thin):
		CreateThinPlatform();
		break;


	default:
		sf::Texture* defaultTexture = FileCache::GetArenaTexture("Textures/default.png");
		defaultTexture->isRepeated();
		_shape.setTexture(defaultTexture);
		break;

	}

	_type = type;

}

Platform::~Platform() {
	std::for_each(_sprites.begin(), _sprites.end(), PlatformSpriteDeallocator());
}

void Platform::Draw(sf::RenderWindow& window) {
	window.draw(_shape);
	for (const auto& sprite : _sprites) {
		window.draw(*sprite);
	}
}

Platform::PlatformType Platform::GetType() {
	return _type;
}


sf::Vector2f Platform::GetSize() {
	return _shape.getSize();
}

int Platform::GetHeight() {
	return _shape.getSize().y;
}

int Platform::GetWidth() {
	return _shape.getSize().x;
}

sf::FloatRect Platform::GetBoundingRect() {
	return _shape.getGlobalBounds();
}

sf::Vector2f Platform::GetCentre() {
	return _shape.getPosition();
}



//Platform Type Constructors

void Platform::CreateThinPlatform() {

	//Sprite text rects:
	//	- Edge: (0,201), 7x7
	//	- Beam: (0,193), 1x7
	//	- Beam shaded by edge: (7,193), 1x7
	//	- Left edge: (0,201), 8x7
	//	- Right edge: (8,193), 8x7
	sf::Texture* Texture = FileCache::GetArenaTexture("Textures/Colorful_pixel_tiles_Tilemap.png");
	
	//If the Thin platform is less than 7 pixels high the texture rects use the platform height.
	int rectHeight;
	if (_height < 7) {
		rectHeight = _height;
	}
	else
		rectHeight = 7;

	sf::IntRect edgeRect = sf::IntRect(0, 201, 7, rectHeight);
	sf::IntRect beamRect = sf::IntRect(0, 193, 1, rectHeight);
	sf::IntRect beamShadeRect = sf::IntRect(7, 193, 1, rectHeight);
	sf::IntRect leftEdgeRect = sf::IntRect(0, 201, 8, rectHeight);
	sf::IntRect rightEdgeRect = sf::IntRect(7, 193, 8, rectHeight);

	sf::Sprite* newSprite;

	float left = _midX - (_width / 2);
	float right = _midX + (_width / 2);
	float top = _midY - (_height / 2);

	//The thin platforms should all be 7 pixels high, if not this pattern just gets repeated.

	//Sprite structure: Edge - Beam Shade - Beam - Beam Shade - Edge.
	//The edges are created first, then the beam shades, with the beam sprites filled in between.

	//Contruct platform 1 row at a time:
	for (int heightRemaining = 0; heightRemaining < _height; heightRemaining += 7) {
		
		//Correct heightRemaining if at bottom of platform.
		if (_height - heightRemaining >= 7) {
			rectHeight = 7;
		}
		else {
			rectHeight = _height - heightRemaining;
		}

		//Update necessary texture rects
		beamRect = sf::IntRect(0, 193, 1, rectHeight);
		leftEdgeRect = sf::IntRect(0, 201, 8, rectHeight);
		rightEdgeRect = sf::IntRect(7, 193, 8, rectHeight);
		
		//Create left edge
		if (_width < 8) {
			newSprite = new sf::Sprite(*Texture, sf::IntRect(0, 201, _width, rectHeight));
			newSprite->setPosition(left, top);
			_sprites.push_back(newSprite);
		}
		else {
			newSprite = new sf::Sprite(*Texture, leftEdgeRect);
			newSprite->setPosition(left, top);
			_sprites.push_back(newSprite);

			//Create right edge
			if (_width < 16) {
				newSprite = new sf::Sprite(*Texture, sf::IntRect(8, 193, _width - 8, rectHeight));
				newSprite->setPosition(right - (_width - 8), top);
				_sprites.push_back(newSprite);
			}
			else {
				newSprite = new sf::Sprite(*Texture, rightEdgeRect);
				newSprite->setPosition(right - 8, top);
				_sprites.push_back(newSprite);

				//Create beams
				for (int widthRemaining = 0; widthRemaining < _width - 16; widthRemaining++) {
					//Beams are 1 pixel wide so rect width doesnt need correcting
					newSprite = new sf::Sprite(*Texture, beamRect);
					newSprite->setPosition(left + 8 + widthRemaining, top);
					_sprites.push_back(newSprite);
				}
			}
		}
	}
}


void Platform::CreateCastleTestPlatform() {
	

	//Sprite text rect dimensions:
	//	- Top: (14,12), 31x20
	//	- Middle: (14,14), 31,18
	sf::Texture* CastleTexture = FileCache::GetArenaTexture("Textures/cstle_tileset 1.png");

	sf::IntRect topRect = sf::IntRect(14, 12, 32, 34);
	sf::IntRect midRect = sf::IntRect(14, 14, 32, 32);

	float left = _midX - (_width / 2);
	float top = _midY - (_height / 2);

	sf::Sprite* newSprite;

	//Fill in sprites 1 column at a time from right to left
	int currentSpriteWidth, currentSpriteHeight;
	for (int widthRemaining = 0; widthRemaining < _width; widthRemaining += 32) {

		if (_width - widthRemaining >= 32)	//If there's enough space for the full texture rect, current sprite width is maximum
			currentSpriteWidth = 32;
		else		//If there's not enough space, current sprite width is equal to the space between the last sprite and the edge of the platform
			currentSpriteWidth = static_cast<int>(floor(_width - widthRemaining));

		//If the platform is only tall enough to fit 1 sprite (the top sprite):
		if (_height <= 34) {
			newSprite = new sf::Sprite(*CastleTexture, sf::IntRect(14, 12, currentSpriteWidth, _height));
			newSprite->setPosition(left + widthRemaining, top);
			_sprites.push_back(newSprite);
		}
		else {	//If the platform is tall enoguh for multiple sprites in a column:

			//Create top sprite:
			newSprite = new sf::Sprite(*CastleTexture, sf::IntRect(14, 12, currentSpriteWidth, 34));
			newSprite->setPosition(left + widthRemaining, top);
			_sprites.push_back(newSprite);

			//Create other sprites:
			for (int heightRemaining = 34; heightRemaining < _height; heightRemaining += 32) {

				if (_height - heightRemaining >= 32)
					currentSpriteHeight = 32;
				else
					currentSpriteHeight = _height - heightRemaining;

				newSprite = new sf::Sprite(*CastleTexture, sf::IntRect(14, 14, currentSpriteWidth, currentSpriteHeight));
				newSprite->setPosition(left + widthRemaining, top + heightRemaining);
				_sprites.push_back(newSprite);
			}
		}
	}
}