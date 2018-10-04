#include "Resources.h"
#include <stdio.h>

std::map<char, sf::Texture> Resources::texture = {};
bool Resources::ready = false;

Resources::Resources()
{
	sf::Texture buffer;
	if (buffer.loadFromFile("grass.png"))
		texture['g'] = buffer;
	if (buffer.loadFromFile("0.png"))
		texture['0'] = buffer;
	if (buffer.loadFromFile("outline.png"))
		texture['o'] = buffer;
	ready = true;
}


Resources::~Resources()
{
}

sf::Texture * Resources::GetTexture(char name)
{
	if (!ready) {
		sf::Texture buffer;
		buffer.loadFromFile("0.png");
		return &buffer;
	}
	if(texture.count(name)>0)
		return &texture.at(name);
	return nullptr;
}
