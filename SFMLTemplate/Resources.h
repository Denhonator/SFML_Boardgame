#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class Resources
{
public:
	Resources();
	~Resources();
	static sf::Texture* GetTexture(std::string name);
	static bool isReady() { return ready; };
private:
	static std::map<std::string, sf::Texture> tile;
	static bool ready;
};

