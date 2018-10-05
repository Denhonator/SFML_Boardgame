#include "Unit.h"

int Unit::unitCount = 0;

Unit::Unit(std::string name, int player)
{
	unitCount++;
	sprite.setTexture(*Resources::GetTexture("units/" + name));
	sprite.setScale(sf::Vector2f(Constants::tileSize / sprite.getLocalBounds().width, Constants::tileSize / sprite.getLocalBounds().height));
	this->player = player;
	id = unitCount;
}

Unit::~Unit()
{
}

void Unit::MoveTo(sf::Vector2i pos, Tile* tile)
{
	if (tile->sprite != "0" && tile->unit==-1) {
		sprite.setPosition(sf::Vector2f(pos.x*Constants::tileSize, pos.y*Constants::tileSize));
		tile->unit = id;
	}
}

