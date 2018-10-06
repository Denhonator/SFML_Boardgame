#include "Tile.h"

Tile* Tile::tileRef[Constants::boardSize];

Tile::Tile()
{
	sprite = "0";
	unit = -1;
}

Tile::Tile(std::string tex)
{
	sprite = tex;
	unit = -1;
}

Tile::~Tile()
{
}

std::string Tile::Print()
{
	return sprite+"\nUnit: "+std::to_string(unit);
}
