#include "MenuElement.h"

MenuElement::MenuElement()
{
	name = "";
	value = 0;
}

MenuElement::MenuElement(std::string name, std::string texName, int val)
{
	this->name = name;
	value = val;
	sprite.setTexture(*Resources::GetTexture(texName));
	text = sf::Text("", *Resources::GetFont("default.ttf"));
}

MenuElement::~MenuElement()
{
}

void MenuElement::SetPosition(sf::Vector2f pos)
{
	sf::Vector2f dif = pos - sprite.getPosition();
	sprite.move(dif);
	text.move(dif);
}

void MenuElement::SetScale(sf::Vector2f scale)
{
	sprite.setScale(scale);
	CenterText();
}

void MenuElement::CenterText()
{
	if (text.getString().getSize()) {
		text.setScale(0.4f*sprite.getGlobalBounds().height / text.getGlobalBounds().height, 0.4f*sprite.getGlobalBounds().height / text.getGlobalBounds().height);
		text.setPosition(std::max(sprite.getPosition().x + sprite.getGlobalBounds().width/2-text.getGlobalBounds().width/2,sprite.getPosition().x),
			sprite.getPosition().y + sprite.getGlobalBounds().height / 2 - text.getGlobalBounds().height);
	}
}
