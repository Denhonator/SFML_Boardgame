#include "Resources.h"

std::map<std::string, sf::Texture> Resources::tile = {};
bool Resources::ready = false;

Resources::Resources()
{
	//sf::Texture buffer;
	//if (buffer.loadFromFile("grass.png"))
	//	tile['g'] = buffer;
	//if (buffer.loadFromFile("0.png"))
	//	tile['0'] = buffer;
	//if (buffer.loadFromFile("outline.png"))
	//	tile['o'] = buffer;
	//ready = true;
}


Resources::~Resources()
{
}

sf::Texture * Resources::GetTexture(std::string name)
{
	/*if (!ready) {
		sf::Texture buffer;
		buffer.loadFromFile("0.png");
		return &buffer;
	}*/
	if(tile.count(name)>0)
		return &tile.at(name);
	sf::Texture buffer;
	if (buffer.loadFromFile(name + ".png")) {
		tile[name] = buffer;
		return &tile[name];
	}
	return &tile.at("0");
}
