#include "Unit.h"

int Unit::unitCount = 0;

Unit::Unit(std::string name, int player)
{
	unitCount++;
	sprite.setTexture(*Resources::GetTexture("units/" + name + "/" + name));
	sprite.setScale(sf::Vector2f(Constants::tileSize / sprite.getLocalBounds().width, Constants::tileSize / sprite.getLocalBounds().height));
	this->player = player;
	this->name = name;
	maxAP = 8;
	AP = maxAP;
	id = unitCount;
	tile = sf::Vector2i(0, 0);
	Tile::tileRef[tile.x][tile.y].unit = id;
}

Unit::~Unit()
{
}

int Distance(sf::Vector2i a, sf::Vector2i b) {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

void Unit::MoveTo(sf::Vector2i pos)
{
	if (Tile::tileRef[pos.x][pos.y].sprite != "0" && Tile::tileRef[pos.x][pos.y].unit==-1) {
		if (std::abs(tile.x - pos.x) < 2 && std::abs(tile.y - pos.y) < 2) {
			if (Distance(pos, tile) == 2) {
				if (AP >= 3)
					AP -= 3;
				else
					return;
			}
			if (Distance(pos, tile) == 1) {
				if (AP >= 2)
					AP -= 2;
				else
					return;
			}
			sprite.setPosition(sf::Vector2f(pos.x*Constants::tileSize, pos.y*Constants::tileSize));
			Tile::tileRef[tile.x][tile.y].unit = -1;
			tile = pos;
			Tile::tileRef[tile.x][tile.y].unit = id;
		}
	}
}

void Unit::EndOfTurn()
{
	AP = maxAP;
}

std::string Unit::Print()
{
	return "Selected: "+name+"\nAP: "+ std::to_string(AP)+"/"+std::to_string(maxAP) +"\nID: "+std::to_string(id);
}

