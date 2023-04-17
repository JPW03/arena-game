#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>


//File cache will store:
//	- Sound files.
//	- Texture maps for animations.
//	- Texture maps for arenas.
//	- Music files?

class FileCache {

public:
	FileCache();
	~FileCache();

	static sf::Texture* GetAnimationTexture(std::string);
	static sf::Texture* GetArenaTexture(std::string);
	static sf::Sound GetSoundEffect(std::string);

	/*
	void AddAnimationTexture(std::string);
	void AddArenaTexture(std::string);
	void AddSoundEffect(std::string);
	*/	//These functions are obselete, the get functions will add files automatically

private:

	static std::map<std::string, sf::Texture*> _animationTextures;
	static std::map<std::string, sf::Texture*> _arenaTextures;
	static std::map<std::string, sf::SoundBuffer*> _soundEffects;


	static sf::Texture* _defaultTexture;
	static sf::Texture* FileMissingException(std::string filename);

	//Deallocator for all maps
	template<typename T>
	struct Deallocator {
		void operator()(const std::pair<std::string, T> &p) {
			delete p.second;
		}
	};
};