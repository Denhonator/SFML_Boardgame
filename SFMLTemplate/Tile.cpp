#include "Tile.h"

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
