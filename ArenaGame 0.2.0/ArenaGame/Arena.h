#pragma once
#include <SFML\Graphics.hpp>
#include "Platform.h"
#include <fstream>


class Arena {

public:

	//Takes name of the arena file, opens it, interprets it, stores the platform data into _platforms
	Arena(std::string arenaFileName);
	
	void Draw(sf::RenderWindow& window);

	sf::Vector2f GetEnemySpawn();	//Returns a random enemy spawn
	sf::Vector2f GetEnemySpawn(int);
	sf::Vector2f GetPlayerSpawn();
	sf::Vector2i GetSize();
	Platform* GetPlatformByIndex(int index);
	Platform* GetPlatformByPosition(sf::Vector2f position);
	int NoOfPlatforms();

	~Arena(void);


private:
	std::vector< Platform* > _platforms;		//Platform container
	sf::Vector2i _size;							//The bounds (width and height) of the arena.
	sf::Vector2i _playerSpawn;					//Where the player spawns in the arena
	std::vector< sf::Vector2i > _enemySpawns;	//Where enemies can spawn in the arena
	sf::Sprite _background;						//Sprite representing the background. Image is placed from the centre

	void GetNextInt(std::string line, int& offset, int& var);
	void GetNextFloat(std::string line, int& offset, float& var);

	//Deallocator to prevent memory leak when loading between arenas.
	struct ArenaPlatformDeallocator {
		void operator() (Platform* &p) const {
			delete p;
		}
	};

};