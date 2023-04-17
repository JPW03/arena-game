#pragma once
#include "SFML\Graphics.hpp"
#include "FileCache.h"


class Platform {

public:
	//Type of platform determines texture and collision (i.e. can you jump through it)
	enum PlatformType { Default, Solid, Thin, CastleTest };

	Platform(sf::Vector2i topLeftCorner, sf::Vector2i size, PlatformType type);
	Platform(float, float, int width, int height, PlatformType type);
	~Platform();

	void Draw(sf::RenderWindow& window);

	sf::FloatRect GetBoundingRect();
	sf::Vector2f GetCentre();
	PlatformType GetType();
	sf::Vector2f GetSize();
	int GetHeight();
	int GetWidth();



private:
	PlatformType _type;

	//Each platform will consist of a rectangle and a series of sprites to fill in that rectangle.
	//These sprites will be procedurally generated in the constructor based on platform type.
	std::vector<sf::Sprite*> _sprites;

	sf::RectangleShape _shape;
	float _height;
	float _width;
	float _midX;
	float _midY;


	//Platform type constructors:
	//(functions that fill platforms with different sprites in different configurations)
	void CreateCastleTestPlatform();
	void CreateThinPlatform();


	struct PlatformSpriteDeallocator {
		void operator() (sf::Sprite* &p) const {
			delete p;
		}
	};
};