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
		attributes.clear();
		attributeTexts.clear();
		items.clear();
		return false;
	}
	Unit* u = Unit::GetUnit(player);
	if (u == nullptr)
		return false;
	this->player = player;
	state = 0;
	bgs.push_back(sf::Sprite(*Resources::GetTexture("ui/menuBG")));
	for (unsigned int i = 0; i < Constants::attributes.size(); i++) {
		attributeTexts.push_back(sf::Text(Constants::attributes.at(i) + ": " + std::to_string(u->GetAttribute(Constants::attributes.at(i))), *Resources::GetFont("default.ttf")));
		attributes.push_back(MenuElement(Constants::attributes.at(i), "ui/button", 1));
		attributes.at(i).text.setString(std::to_string(u->GetAtrRequirment(Constants::attributes.at(i)))+"XP");
	}
	for (unsigned int i = 0; i < u->GetWeapons()->size(); i++) {
		items.push_back(MenuElement("weapon", u->GetWeapon(i)->inUse ? "ui/item" : "ui/button", i));
		items.at(i).text.setString(u->GetWeapons()->at(i).Print(false,true));
	}
	for (unsigned int i = 0; i < u->GetEquipment()->size(); i++) {
		items.push_back(MenuElement("equipment", u->GetEquipment()->at(i).inUse ? "ui/item" : "ui/button", i));
		items.at(items.size() - 1).text.setString(u->GetEquipment()->at(i).Print(false, true));
	}
	for (unsigned int i = 0; i < u->GetItems()->size(); i++) {
		items.push_back(MenuElement("item", "ui/button", i));
		items.at(items.size() - 1).text.setString(u->GetItems()->at(i).Print());
	}
	attributeTexts.push_back(sf::Text("XP: " + u->PrintStats("XP"),*Resources::GetFont("default.ttf")));
	return true;
}

void Menu::SetRect(sf::FloatRect r)
{
	rect = r;
	bgs.at(0).setScale(sf::Vector2f(rTex.getSize().x / bgs.at(0).getLocalBounds().width, rTex.getSize().y / bgs.at(0).getLocalBounds().height));

	for (unsigned int i = 0; i < attributes.size(); i++) {
		attributes.at(i).SetScale(sf::Vector2f(1, 0.5f));
		attributes.at(i).CenterText(attributes.at(0).text.getGlobalBounds());
		attributes.at(i).text.setStyle(sf::Text::Bold);
		attributeTexts.at(i).setPosition(sf::Vector2f(30, 50 + i * attributes.at(i).sprite.getGlobalBounds().height));
		attributeTexts.at(i).setStyle(sf::Text::Bold);
		attributes.at(i).SetPosition(sf::Vector2f(350, 40 + i * attributes.at(i).sprite.getGlobalBounds().height));
	}
	attributeTexts.at(attributeTexts.size() - 1).setPosition(attributeTexts.at(attributeTexts.size() - 2).getPosition() + sf::Vector2f(20, 90));
	attributeTexts.at(attributeTexts.size() - 1).setStyle(sf::Text::Bold);

	for (unsigned int i = 0; i < items.size(); i++) {
		items.at(i).SetScale(sf::Vector2f(4, 0.5f));
		items.at(i).CenterText(items.at(0).text.getGlobalBounds());
		items.at(i).text.setStyle(sf::Text::Bold);
		items.at(i).SetPosition(sf::Vector2f(500, 40 + i * items.at(i).sprite.getGlobalBounds().height));
	}
	update = true;
	draw = true;
}

sf::Sprite * Menu::Draw()
{
	if (update) {
		rTex.clear(sf::Color(0,0,0,0));
		//rTex.create(rect.width, rect.height);
		for (unsigned int i = 0; i < bgs.size(); i++) {
			rTex.draw(bgs.at(i));
		}
		for (unsigned int i = 0; i < attributes.size(); i++) {
			rTex.draw(attributes.at(i).sprite);
			rTex.draw(attributes.at(i).text);
		}
		for (unsigned int i = 0; i < items.size(); i++) {
			rTex.draw(items.at(i).sprite);
			rTex.draw(items.at(i).text);
		}
		for (unsigned int i = 0; i < attributeTexts.size(); i++) {
			rTex.draw(attributeTexts.at(i));
		}
		rTex.display();
		rTexSprite.setTexture(rTex.getTexture());
		rTexSprite.setPosition(rect.left, rect.top);
		update = false;
	}
	return &rTexSprite;
}

sf::Vector2f Menu::GetOffset()
{
	return sf::Vector2f(rect.left, rect.top);
}

void Menu::Refresh()
{
	Unit* u = Unit::GetUnit(player);
	if (u == nullptr)
		return;
	unsigned int c = 0;
	for (unsigned int i = 0; i < Constants::attributes.size(); i++) {
		attributes.at(i).text.setString(std::to_string(u->GetAtrRequirment(Constants::attributes.at(i))) + "XP");
		attributeTexts.at(i).setString(Constants::attributes.at(i) + ": " + std::to_string(u->GetAttribute(Constants::attributes.at(i))));
	}
	for (unsigned int i = 0; i < u->GetWeapons()->size(); i++) {
		items.at(i).text.setString(u->GetWeapons()->at(i).Print(false, true));
		items.at(i).sprite.setTexture(u->GetWeapon(i)->inUse ? *Resources::GetTexture("ui/item") : *Resources::GetTexture("ui/button"));
		c++;
	}
	for (unsigned int i = 0; i < u->GetEquipment()->size(); i++) {
		items.at(c).text.setString(u->GetEquipment()->at(i).Print(false, true));
		items.at(c).sprite.setTexture(u->GetEquipment()->at(i).inUse ? *Resources::GetTexture("ui/item") : *Resources::GetTexture("ui/button"));
		c++;
	}
	for (unsigned int i = 0; i < u->GetItems()->size(); i++) {
		items.at(c).text.setString(u->GetItems()->at(i).Print());
		c++;
	}
	attributeTexts.at(attributeTexts.size()-1).setString("XP: " + u->PrintStats("XP"));
	update = true;
}
