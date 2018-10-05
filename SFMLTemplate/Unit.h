#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Resources.h"
#include "Constants.h"
#include "Tile.h"

class Unit
{
public:
	Unit(std::string name, int player=0);
	~Unit();
	void MoveTo(sf::Vector2i pos, Tile* tile);
	int player;
	int id;
	sf::Sprite sprite;
private:
	std::string name;
	static int unitCount;
};
