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
	void MoveTo(sf::Vector2i pos);
	void EndOfTurn();
	std::string Print();
	int player;
	int id;
	sf::Vector2i tile;
	sf::Sprite sprite;
private:
	std::string name;
	static int unitCount;
	int maxAP;
	int AP;
};
