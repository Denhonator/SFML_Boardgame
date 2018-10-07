#include "Unit.h"

int Unit::unitCount = 0;

Unit::Unit(std::string name, int player)
{
	unitCount++;
	sprite.setTexture(*Resources::GetTexture("units/" + name + "/" + name));
	sprite.setScale(sf::Vector2f(Constants::tileSize / sprite.getLocalBounds().width, Constants::tileSize / sprite.getLocalBounds().height));
	this->player = player;
	this->name = name;
	id = unitCount;
	tile = sf::Vector2i(0, 0);
	Tile::tileRef[tile.x][tile.y].unit = id;
}

Unit::~Unit()
{
}

void Unit::MoveTo(sf::Vector2i pos)
{
	if (Tile::tileRef[pos.x][pos.y].sprite != "0" && Tile::tileRef[pos.x][pos.y].unit==-1) {
		if (std::abs(tile.x - pos.x) < 2 && std::abs(tile.y - pos.y) < 2) {
			sprite.setPosition(sf::Vector2f(pos.x*Constants::tileSize, pos.y*Constants::tileSize));
			Tile::tileRef[tile.x][tile.y].unit = -1;
			tile = pos;
			Tile::tileRef[tile.x][tile.y].unit = id;
		}
	}
}

std::string Unit::Print()
{

	return "Selected: "+name+"\n"+std::to_string(id);
}

