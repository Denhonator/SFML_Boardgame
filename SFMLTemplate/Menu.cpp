#include "Menu.h"

Menu::Menu()
{
}

Menu::Menu(int state, int player)
{
	rect = sf::FloatRect(0, 0, Constants::fixedView.width, Constants::fixedView.height);
	this->state = state;
	this->player = player;
	draw = false;
	if (player > 0) {
		Construct(player);
	}
}

Menu::~Menu()
{
}

bool Menu::Construct(int player)
{
	if (player == this->player) {
		state = -1;
		this->player = -1;
		draw = false;
		elements.clear();
		return false;
	}
	Unit* u = Unit::GetUnit(player);
	if (u == nullptr)
		return false;
	this->player = player;
	state = 0;
	elements.clear();
	elements.push_back(MenuElement("bg", "ui/menuBG", 0));
	for (int i = 0; i < Constants::attributes.size(); i++) {
		elements.push_back(MenuElement(Constants::attributes.at(i), "ui/button", 1));
		elements.at(i+1).text.setString(Constants::attributes.at(i)+": "+std::to_string(u->GetAttribute(Constants::attributes.at(i))));
	}
	draw = true;
	return true;
}

void Menu::SetRect(sf::FloatRect r)
{
	rect = r;
	sf::Vector2f scale = sf::Vector2f(r.width / Constants::fixedView.width, r.height / Constants::fixedView.height);
	elements.at(0).SetScale(sf::Vector2f(r.width / elements.at(0).sprite.getLocalBounds().width, r.height / elements.at(0).sprite.getLocalBounds().height));
	elements.at(0).SetPosition(sf::Vector2f(r.left, r.top));
	for (int i = 1; i < elements.size(); i++) {
		elements.at(i).SetScale(sf::Vector2f(scale.x * 5, scale.y*0.5f));
		elements.at(i).SetPosition(sf::Vector2f(r.left + 20, r.top + 40 + (i - 1) * elements.at(i).sprite.getGlobalBounds().height));
	}
}
