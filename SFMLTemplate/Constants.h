#pragma once
#include <SFML/Graphics.hpp>
class Constants
{
public:
	Constants();
	~Constants();
	static void UpdateView(sf::View v);
	static const int boardSize = 40;
	static float tileSize;
	static std::vector<std::string> attributes;
	static sf::FloatRect fixedView;
};

