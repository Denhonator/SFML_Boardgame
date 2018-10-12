#include "Constants.h"

float Constants::tileSize;
std::vector<std::string> Constants::attributes = { "strength","dexterity","perception","charisma","willpower","intelligence","vitality","luck" };
sf::FloatRect Constants::fixedView;

Constants::Constants()
{
}

Constants::~Constants()
{
}

void Constants::UpdateView(sf::View v)
{
	fixedView = sf::FloatRect(v.getCenter().x - v.getSize().x / 2, v.getCenter().y - v.getSize().y / 2, v.getSize().x, v.getSize().y);
}
