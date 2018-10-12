#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include "Constants.h"
#include <thread>

class Resources
{
public:
	Resources();
	~Resources();
	static sf::Texture* GetTexture(std::string name);
	static sf::Font* GetFont(std::string name);
	static std::vector<std::string> GetText(std::string path);
	static void PlayWav(std::string s);
	static std::pair<std::string, std::vector<int>> KeyWithInts(std::string line);
	static std::pair<std::string, std::vector<std::string>> KeyWithStrings(std::string line);
	static bool StrInVector(std::string s, std::vector<std::string> v);
	static short Roll();
private:
	void RollerFunction();
	static std::map<std::string, sf::Texture> tile;
	static std::map<std::string, sf::Font> font;
	static std::map<std::string, sf::SoundBuffer> sound;
	static std::vector<sf::Sound> player;
	static short roll;
	std::thread roller;
	bool running;
};

struct Damage {
	short physical, fire, ice, lightning;
};

