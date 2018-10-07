#include "Resources.h"

std::map<std::string, sf::Texture> Resources::tile = {};
std::map<std::string, sf::Font> Resources::font = {};
bool Resources::ready = false;


Resources::Resources()
{
	srand(time(NULL));
}


Resources::~Resources()
{
}

sf::Texture * Resources::GetTexture(std::string name)
{
	if(tile.count(name)>0)
		return &tile.at(name);
	sf::Texture buffer;
	if (buffer.loadFromFile(name + ".png")) {
		tile[name] = buffer;
		return &tile[name];
	}
	return &tile.at("0");
}

sf::Font * Resources::GetFont(std::string name)
{
	if (font.count(name) > 0)
		return &font.at(name);
	sf::Font buffer;
	if (buffer.loadFromFile(name)) {
		font[name] = buffer;
		return &font[name];
	}
	return nullptr;
}

short Resources::Roll()
{
	return rand()%100+1;
}
