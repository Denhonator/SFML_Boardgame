#pragma once
#include <vector>
class Constants
{
public:
	Constants();
	~Constants();
	static const unsigned short boardSize = 40;
	static float tileSize;
	static std::vector<std::string> attributes;
};

