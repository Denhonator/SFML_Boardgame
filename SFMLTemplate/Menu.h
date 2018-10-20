#pragma once
#include "MenuElement.h"

class Menu
{
public:
	Menu();
	Menu(int state, int player);
	~Menu();
	bool Construct(int player);
	void SetRect(sf::FloatRect r);
	std::vector<MenuElement> elements;
	int player;
	int state;
	bool draw;
private:
	sf::FloatRect rect;
};

