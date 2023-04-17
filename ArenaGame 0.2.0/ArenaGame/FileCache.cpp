#include "FileCache.h"
#include <algorithm>


FileCache::FileCache() {
}


FileCache::~FileCache() {

	std::for_each(_animationTextures.begin(), _animationTextures.end(), Deallocator<sf::Texture*>());
	std::for_each(_arenaTextures.begin(), _arenaTextures.end(), Deallocator<sf::Texture*>());
	std::for_each(_soundEffects.begin(), _soundEffects.end(), Deallocator<sf::SoundBuffer*>());

}


sf::Texture* FileCache::GetAnimationTexture(std::string filename) {

	//Check if the texture is already cached:
	std::map<std::string, sf::Texture*>::iterator itr = _animationTextures.find(filename);

	if (itr == _animationTextures.end()) {	//If the texture isn't cached

		//Load texture
		sf::Texture* newTexture = new sf::Texture();
		if (!newTexture->loadFromFile(filename)) {
			throw (std::runtime_error("FileCache::GetAnimationTexture - " + filename + " texture was not found."));
		}	//Throw exception if the file can't be loaded.
		
		//Add texture to the map.
		_animationTextures.insert(std::pair<std::string, sf::Texture*>(filename, newTexture));

		//Working version?
		//std::map<std::string, sf::Texture*>::iterator res = _animationTextures.insert(std::pair<std::string, sf::Texture*>(filename, newTexture)).first;

		//Return the new texture.
		return newTexture;
	}
	else {
		return itr->second;
	}
}


sf::Texture* FileCache::GetArenaTexture(std::string filename) {

	//Check if the texture is already cached:
	std::map<std::string, sf::Texture*>::iterator itr = _arenaTextures.find(filename);

	if (itr == _arenaTextures.end()) {	//If the texture isn't cached

		//Load texture
		sf::Texture* newTexture = new sf::Texture();
		if (!newTexture->loadFromFile(filename)) {
			throw (std::runtime_error("FileCache::GetArenaTexture - " + filename + " texture was not found."));
		}	//Throw exception if the file can't be loaded.

		//Add texture to the map.
		_arenaTextures.insert(std::pair<std::string, sf::Texture*>(filename, newTexture));

		//Working version?
		//std::map<std::string, sf::Texture*>::iterator res = _animationTextures.insert(std::pair<std::string, sf::Texture*>(filename, newTexture)).first;

		//Return the new texture.
		return newTexture;
	}
	else {
		return itr->second;
	}
}


sf::Sound FileCache::GetSoundEffect(std::string filename) {

	//Check if the texture is already cached:
	std::map<std::string, sf::SoundBuffer*>::iterator itr = _soundEffects.find(filename);

	if (itr == _soundEffects.end()) {	//If the texture isn't cached

		//Load sound buffer
		sf::SoundBuffer* newSoundBuffer = new sf::SoundBuffer();
		if (!newSoundBuffer->loadFromFile(filename)) {
			throw (std::runtime_error("FileCache::GetSoundEffect - " + filename + " sound was not found."));
		}	//Throw exception if the file can't be loaded.

		//Add texture to the map.
		_soundEffects.insert(std::pair<std::string, sf::SoundBuffer*>(filename, newSoundBuffer));

		//Working version?
		//std::map<std::string, sf::Texture*>::iterator res = _animationTextures.insert(std::pair<std::string, sf::Texture*>(filename, newTexture)).first;

		//Return the new sound.
		sf::Sound newSound;
		newSound.setBuffer(*newSoundBuffer);
		return newSound;
	}
	else {
		sf::Sound newSound;
		newSound.setBuffer(*itr->second);
		return newSound;
	}
}



//Declare static variables:
std::map<std::string, sf::Texture*> FileCache::_animationTextures;
std::map<std::string, sf::Texture*> FileCache::_arenaTextures;
std::map<std::string, sf::SoundBuffer*> FileCache::_soundEffects;
sf::Texture* FileCache::_defaultTexture;