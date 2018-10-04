#pragma once
#include <SFML/Graphics.hpp>
class Resources
{
public:
	Resources();
	~Resources();
	static sf::Texture* GetTexture(char name);
	static bool isReady() { return ready; };
private:
	static std::map<char, sf::Texture> texture;
	static bool ready;
};

