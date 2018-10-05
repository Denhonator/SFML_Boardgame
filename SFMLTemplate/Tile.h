#pragma once
#include <string>
class Tile
{
public:
	Tile();
	Tile(std::string tex);
	~Tile();
	std::string sprite;
	int unit;
};

