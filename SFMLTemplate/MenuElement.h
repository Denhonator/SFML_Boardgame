#pragma once
#include "Resources.h"
#include "Unit.h"

class MenuElement
{
public:
	MenuElement();
	MenuElement(std::string name, std::string texName, int val);
	~MenuElement();
	void SetPosition(sf::Vector2f pos);
	void SetScale(sf::Vector2f scale);
	void CenterText(sf::FloatRect bounds);
	sf::Sprite sprite;
	sf::Text text;
	std::string name;
	int value;
};

