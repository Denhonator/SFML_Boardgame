#pragma once
#include <string>
#include "Constants.h"
class Tile
{
public:
	Tile();
	Tile(std::string tex);
	~Tile();
	std::string Print();
	std::string sprite;
	static Tile* tileRef[Constants::boardSize];
	short unit;
	std::vector<short> otherUnits;
};

