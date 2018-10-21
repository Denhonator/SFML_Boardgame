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
}

void MenuElement::CenterText(sf::FloatRect bounds)
{
	if (text.getString().getSize()) {
		text.setPosition(sprite.getPosition().x+15,
			sprite.getPosition().y + sprite.getGlobalBounds().height / 2 - bounds.height);
		//text.setScale(0.4f*sprite.getGlobalBounds().width / text.getGlobalBounds().width, 0.4f*sprite.getGlobalBounds().width / text.getGlobalBounds().width);
	}
}
