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
	std::vector<MenuElement> attributes;
	std::vector<MenuElement> items;
	std::vector<sf::Text> attributeTexts;
	std::vector<sf::Sprite> bgs;
	sf::Sprite* Draw();
	sf::Vector2f GetOffset();
	void Refresh();
	int player;
	int state;
	bool draw;
private:
	bool update;
	sf::FloatRect rect;
	static sf::RenderTexture rTex;
	sf::Sprite rTexSprite;
};

