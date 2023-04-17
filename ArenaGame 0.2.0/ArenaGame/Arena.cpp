#include "Arena.h"
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "FileCache.h"



Arena::~Arena(void) {
	std::for_each(_platforms.begin(), _platforms.end(), ArenaPlatformDeallocator());
}



Arena::Arena(std::string filename) {

	std::ifstream ArenaFile(filename);

	//If the file isn't found, throw an exception. Stop constructor
	if (!ArenaFile.is_open())
		throw(std::runtime_error("Arena file (" + filename + ") was not found."));

	//First line of file is always the size of the arena:
	std::string fileLine;
	std::getline(ArenaFile, fileLine);

	//Get the width and height of the arena:
	int comma = fileLine.find(',');

	//Get the width (x) of the arena:
	int sizeX = 0;
	for (int i = 0; i < comma; i++) {
		sizeX = sizeX * 10 + (fileLine.substr(0, comma)[i] - 48);
	}

	//Get the height (y) of the arena:
	int sizeY = 0;
	for (int i = 0; i < fileLine.length() - (comma + 1); i++) {
		sizeY = sizeY * 10 + (fileLine.substr(comma + 1)[i] - 48);
	}

	_size.x = sizeX;
	_size.y = sizeY;


	//Run through the rest of the file.
	//	- Load data for spawn points and platforms.
	Platform* newPlatform;
	sf::Vector2i newEnemySpawn;
	bool playerSpawnFound = false;
	bool backgroundFound = false;
	while (!ArenaFile.eof()) {
		
		//Get next line.
		std::getline(ArenaFile, fileLine);

		int next = 1;

		//Load platform: (line starts with #)
		if (fileLine[0] == '#') {
			//Get the top coord of the platform:
			float midX;
			GetNextFloat(fileLine, next, midX);

			//Get the midX coord of the platform:
			float midY;
			GetNextFloat(fileLine, next, midY);

			//Get the width of the platform:
			int width;
			GetNextInt(fileLine, next, width);

			//Get the height of the platform:
			int height;
			GetNextInt(fileLine, next, height);

			//Get the type of the platform:
			int typeN;
			GetNextInt(fileLine, next, typeN);
			Platform::PlatformType type = static_cast<Platform::PlatformType>(typeN);


			//Correct the platform in case it goes outside of the bounds of the arena
			if (midY + (height / 2) > _size.y) {	//Below the arena
				height -= (midY + (height / 2)) - _size.y;
				midY = midY - (height / 2);
			}
			if (midX + (width / 2) > _size.x) {		//Right of the arena
				width -= (midX + (width / 2)) - _size.x;
				midX = midX - (width / 2);
			}
			if (midY - (height / 2) < 0) {			//Above the arena
				height -= -(midY - (height / 2));
				midY = midY + (height / 2);
			}
			if (midX - (width / 2) < 0) {			//Left of the arena
				width -= -(midX - (width / 2));
				midX = midX + (width / 2);
			}

			newPlatform = new Platform(midX, midY, width, height, type);
			_platforms.push_back(newPlatform);
		}

		//Load player spawn (if a player spawn hasn't already been loaded):
		if (fileLine[0] == 'p' && !playerSpawnFound) {

			//Get x of the player spawn:
			GetNextInt(fileLine, next, _playerSpawn.x);
			//Get y of the player spawn:
			GetNextInt(fileLine, next, _playerSpawn.y);

			playerSpawnFound = true;
		}

		//Load enemy spawn:
		if (fileLine[0] == 'e') {

			//Get x of the player spawn:
			GetNextInt(fileLine, next, newEnemySpawn.x);
			//Get y of the player spawn:
			GetNextInt(fileLine, next, newEnemySpawn.y);

			_enemySpawns.push_back(newEnemySpawn);
		}

		//Load background image
		if (fileLine[0] == 'b' && !backgroundFound) {

			sf::Texture* backgroundTexture = FileCache::GetArenaTexture(fileLine.substr(1));

			_background.setOrigin(_size.x / 2, _size.y / 2);
			_background.setPosition(_size.x / 2, _size.y / 2);
			_background.setTexture(*backgroundTexture);

		}

		//If the line starts with a character that's not 'e', 'p' or '#', the rest of the line will be ignored.
	}
}


//For getting the next int in a line string from a file. Used by Arena constructor to get platform data.
void Arena::GetNextInt(std::string line, int& offset, int& var) {
	var = 0;
	while (line[offset] != ',' && line[offset] != ';') {
		var = var * 10 + (line[offset] - 48);
		offset += 1;
	}
	offset += 1;
}

void Arena::GetNextFloat(std::string line, int& offset, float& var) {
	std::string number;
	while (line[offset] != ',' && line[offset] != ';') {
		number += line[offset];
		offset += 1;
	}
	var = std::stof(number);
	offset += 1;
}


sf::Vector2i Arena::GetSize() {
	return _size;
}

sf::Vector2f Arena::GetPlayerSpawn() {
	return sf::Vector2f(static_cast<float>(_playerSpawn.x), static_cast<float>(_playerSpawn.y));
}

int Arena::NoOfPlatforms() {
	return _platforms.size();
}


Platform* Arena:: GetPlatformByIndex(int index) {
	std::vector<Platform*>::iterator itr = _platforms.begin();
	itr += index;
	return *itr;
}

Platform* Arena::GetPlatformByPosition(sf::Vector2f position) {
	std::vector<Platform*>::iterator itr = _platforms.begin();
	
	for (const auto& platform : _platforms) {
		if (platform->GetBoundingRect().contains(position)) {
			return platform;
		}
	}

	return NULL;
}


sf::Vector2f Arena::GetEnemySpawn() {
	//Get a random number between 0 and the number of enemy spawns - 1
	std::srand((unsigned) std::clock());
	int index = std::rand() % (_enemySpawns.size() - 1);

	return GetEnemySpawn(index);
}


sf::Vector2f Arena::GetEnemySpawn(int index) {
	std::vector<sf::Vector2i>::iterator itr = _enemySpawns.begin();

	bool found = false;

	for (int pos = 0; pos < index; pos++) {
		itr++;
		if (itr == _enemySpawns.end()) {
			throw(std::runtime_error("GetEnemySpawn: Tried to find enemy spawn of index " + index));
		}
	}

	return sf::Vector2f(float((*itr).x), float((*itr).y));
}





void Arena::Draw(sf::RenderWindow& window) {

	window.draw(_background);

	for (const auto& current : _platforms) {
		current->Draw(window);
	}
	
}