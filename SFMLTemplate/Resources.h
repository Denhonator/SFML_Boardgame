#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <random>

class Resources
{
public:
	Resources();
	~Resources();
	static sf::Texture* GetTexture(std::string name);
	static sf::Font* GetFont(std::string name);
	static std::vector<std::string> GetText(std::string path);
	static std::pair<std::string, std::vector<int>> KeyWithInts(std::string line);
	static std::pair<std::string, std::vector<std::string>> KeyWithStrings(std::string line);
	static bool StrInVector(std::string s, std::vector<std::string> v);
	static short Roll();
	static bool isReady() { return ready; };
private:
	static std::map<std::string, sf::Texture> tile;
	static std::map<std::string, sf::Font> font;
	static bool ready;
};

struct Damage {
	short physical, fire, ice, lightning;
};

