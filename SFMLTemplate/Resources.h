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
	static short Roll();
	static bool isReady() { return ready; };
private:
	static std::map<std::string, sf::Texture> tile;
	static std::map<std::string, sf::Font> font;
	static bool ready;
};

