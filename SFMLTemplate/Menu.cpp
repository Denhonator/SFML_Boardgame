#include "Menu.h"

sf::RenderTexture Menu::rTex;

Menu::Menu()
{
}

Menu::Menu(int state, int player)
{
	rTex.create(1024, 1024);
	rect = sf::FloatRect(0, 0, rTex.getSize().x, rTex.getSize().y);
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
		bgs.clear();
		elements.clear();
		texts.clear();
		return false;
	}
	Unit* u = Unit::GetUnit(player);
	if (u == nullptr)
		return false;
	this->player = player;
	state = 0;
	elements.clear();
	texts.clear();
	bgs.push_back(sf::Sprite(*Resources::GetTexture("ui/menuBG")));
	for (int i = 0; i < Constants::attributes.size(); i++) {
		texts.push_back(sf::Text(Constants::attributes.at(i) + ": " + std::to_string(u->GetAttribute(Constants::attributes.at(i))), *Resources::GetFont("default.ttf")));
		elements.push_back(MenuElement(Constants::attributes.at(i), "ui/button", 1));
		elements.at(i).text.setString(std::to_string(u->GetAtrRequirment(Constants::attributes.at(i)))+"XP");
	}
	texts.push_back(sf::Text("XP: " + u->PrintStats("XP"),*Resources::GetFont("default.ttf")));
	return true;
}

void Menu::SetRect(sf::FloatRect r)
{
	rect = r;
	bgs.at(0).setScale(sf::Vector2f(rTex.getSize().x / bgs.at(0).getLocalBounds().width, rTex.getSize().y / bgs.at(0).getLocalBounds().height));

	for (int i = 0; i < elements.size(); i++) {
		elements.at(i).SetScale(sf::Vector2f(1, 0.5f));
		elements.at(i).CenterText(elements.at(0).text.getGlobalBounds());
		elements.at(i).text.setStyle(sf::Text::Bold);
		texts.at(i).setPosition(sf::Vector2f(30, 50 + i * elements.at(i).sprite.getGlobalBounds().height));
		texts.at(i).setStyle(sf::Text::Bold);
		elements.at(i).SetPosition(sf::Vector2f(350, 40 + i * elements.at(i).sprite.getGlobalBounds().height));
	}
	texts.at(texts.size() - 1).setPosition(texts.at(texts.size() - 2).getPosition() + sf::Vector2f(20, 90));
	texts.at(texts.size() - 1).setStyle(sf::Text::Bold);
	update = true;
	draw = true;
}

sf::Sprite * Menu::Draw()
{
	if (update) {
		rTex.clear();
		rTex.create(rect.width, rect.height);
		for (unsigned int i = 0; i < bgs.size(); i++) {
			rTex.draw(bgs.at(i));
		}
		for (unsigned int i = 0; i < elements.size(); i++) {
			rTex.draw(elements.at(i).sprite);
			rTex.draw(elements.at(i).text);
		}
		for (unsigned int i = 0; i < texts.size(); i++) {
			rTex.draw(texts.at(i));
		}
		rTex.display();
		rTexSprite.setTexture(rTex.getTexture());
		rTexSprite.setPosition(rect.left, rect.top);
		update = false;
		//rTexSprite.setScale(rect.width / rTexSprite.getLocalBounds().width, rect.width / rTexSprite.getLocalBounds().width);
	}
	return &rTexSprite;
}

sf::Vector2f Menu::GetOffset()
{
	return sf::Vector2f(rect.left, rect.top);
}
