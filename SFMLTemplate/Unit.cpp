#include "Unit.h"

int Unit::unitCount = 0;
std::vector<Unit>* Unit::unit;

Unit::Unit(std::string name, int player, std::string nick)
{
	unitCount++;
	sprite.setTexture(*Resources::GetTexture("units/" + name + "/" + name));
	sprite.setScale(sf::Vector2f(Constants::tileSize / sprite.getLocalBounds().width, Constants::tileSize / sprite.getLocalBounds().height));
	this->player = player;
	this->name = name;
	if (nick == "")
		nick = name;
	this->nick = nick;
	XP = 0;
	LoadFromFile("units/" + name + "/" + nick);
	maxAP = 14+attribute["dexterity"];
	maxHP = 3*attribute["vitality"];
	maxMP = 3 * attribute["intelligence"];
	AP = maxAP;
	HP = maxHP;
	MP = maxMP;
	AddWeapon("self", 1);
	currentWeapon = 0;
	id = unitCount;
	tile = sf::Vector2i(0, 0);
	Tile::tileRef[tile.x][tile.y].unit = id;
	bars = sf::VertexArray(sf::PrimitiveType::TriangleStrip, 8);
	UpdateBars();
}

Unit::~Unit()
{
}

Unit * Unit::GetUnit(int id)
{
	for (int i = 0; i < unit->size(); i++) {
		if (unit->at(i).id == id)
			return &unit->at(i);
	}
	return nullptr;
}

void Unit::LoadFromFile(std::string path)
{
	std::vector<std::string> info = Resources::GetText(path);
	std::string buffer = "";
	for (int i = 0; i < info.size(); i++) {
		std::pair<std::string, std::vector<std::string>> temp = Resources::KeyWithStrings(info.at(i));
		if (Resources::StrInVector(temp.first, Constants::attributes)) {
			attribute[temp.first] = std::stoi(temp.second.at(0));
			attributeGain[temp.first] = std::stoi(temp.second.at(1));
		}
		else if (temp.first == "weapon") {
			AddWeapon(temp.second.at(0), std::stoi(temp.second.at(1)));
		}
		else if (temp.first == "xp") {
			XP = std::stoi(temp.second.at(0));
		}
	}
}

void Unit::AddWeapon(std::string name, short level)
{
	weapons.push_back(Weapon(name, level));
}

void Unit::SwitchWeapon(short i)
{
	if (AP < 2 || weapons.size()==1) {
		if (AP < 2)
			Messages::Notice("Not enough AP to switch weapon");
		else
			Messages::Notice("You only have one weapon");
		return;
	}
	if (i >= 0 && i < weapons.size())
		currentWeapon = i;
	else {
		currentWeapon++;
		if (currentWeapon >= weapons.size())
			currentWeapon = 0;
	}
	AP -= 2;
}

Weapon * Unit::GetWeapon(short i)
{
	if (i<0 || i>weapons.size()) {
		i = currentWeapon;
	}
	return &weapons.at(i);
}

int Distance(sf::Vector2i a, sf::Vector2i b) {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

void Unit::MoveTo(sf::Vector2i pos)
{
	if (Tile::tileRef[pos.x][pos.y].sprite != "0" && Tile::tileRef[pos.x][pos.y].unit==-1) {
		if (std::abs(tile.x - pos.x) < 2 && std::abs(tile.y - pos.y) < 2) {
			if (Distance(pos, tile) == 2) {
				if (AP >= 3)
					AP -= 3;
				else
					return;
			}
			if (Distance(pos, tile) == 1) {
				if (AP >= 2)
					AP -= 2;
				else
					return;
			}
			sprite.setPosition(sf::Vector2f(pos.x*Constants::tileSize, pos.y*Constants::tileSize));
			Tile::tileRef[tile.x][tile.y].unit = -1;
			tile = pos;
			Tile::tileRef[tile.x][tile.y].unit = id;
			UpdateBars();
		}
	}
}

void Unit::AttackTo(sf::Vector2i pos)
{
	if (weapons.at(currentWeapon).CanUse(attribute)) {
		Attack a = weapons.at(currentWeapon).GetAttack();
		if (AP >= a.ap && Distance(tile, pos) <= a.range) {
			Unit* target = GetUnit(Tile::tileRef[pos.x][pos.y].unit);
			if (target != nullptr) {
				Messages::Prompt("Attack friendly unit " + target->nick + "?");
				while (target->player==player) {
					sf::sleep(sf::milliseconds(2));
					sf::Keyboard::Key key = Messages::WaitInput();
					if (key == sf::Keyboard::Key::Y)
						break;
					else if (key != sf::Keyboard::Key::Unknown)
						return;
				}
				AP -= a.ap;
				Messages::Add(name + " rolls " + std::to_string(a.roll) + "/" + std::to_string(a.successThreshold) + " on " + a.name + " against " + target->nick);
				target->GetAttacked(a);
			}
			else {
				Messages::Notice("Invalid target");
			}
		}
	}
	else {
		Messages::Notice("Unable to use current weapon");
	}
}

void Unit::GetAttacked(Attack a)
{
	if (a.successful) {
		HP -= a.damage.physical;
		UpdateBars();
		Resources::PlayWav("hit");
		Messages::Add(name + " takes " + std::to_string(a.damage.physical) + " damage");
	}
	else {
		Resources::PlayWav("woosh");
	}
}

void Unit::EndOfTurn()
{
	AP = maxAP;
}

std::string Unit::Print()
{
	return nick + " (ID: " + std::to_string(id) + ")" +
		"\nHP: " + std::to_string(HP) + "/" + std::to_string(maxHP) +
		"\nAP: " + std::to_string(AP) + "/" + std::to_string(maxAP) +
		"\nMP: " + std::to_string(MP) + "/" + std::to_string(maxMP) +
		"\n" + weapons.at(currentWeapon).Print();
}

void Unit::UpdateBars()
{
	float width = Constants::tileSize / 30;
	float heigth = Constants::tileSize*std::pow(maxHP/30.0f,1.0f);
	sf::Vector2f offset = sf::Vector2f(2, Constants::tileSize / 2 - heigth / 2);
	float line = heigth * (1.0f-((float)HP / (float)maxHP));
	bars[0].position = sprite.getPosition() + offset;
	bars[1].position = sprite.getPosition() + offset + sf::Vector2f(width, 0);
	bars[2].position = sprite.getPosition() + offset + sf::Vector2f(0, line);
	bars[3].position = sprite.getPosition() + offset + sf::Vector2f(width, line);
	bars[4].position = sprite.getPosition() + offset + sf::Vector2f(0, line);
	bars[5].position = sprite.getPosition() + offset + sf::Vector2f(width, line);
	bars[6].position = sprite.getPosition() + offset + sf::Vector2f(0, heigth);
	bars[7].position = sprite.getPosition() + offset + sf::Vector2f(width, heigth);
	for (short i = 0; i < 8; i++) {
		bars[i].color = sf::Color(i<4?0:200, 0, 0, 255);
	}
}

