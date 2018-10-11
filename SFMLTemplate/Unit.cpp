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
	}
}

void Unit::AddWeapon(std::string name, short level)
{
	weapons.push_back(Weapon(name, level));
}

void Unit::SwitchWeapon(short i)
{
	if (AP < 2 || weapons.size()==1) {
		return;
	}
	AP-=2;
	if (i >= 0 && i < weapons.size())
		currentWeapon = i;
	else {
		currentWeapon++;
		if (currentWeapon >= weapons.size())
			currentWeapon = 0;
	}
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
		}
	}
}

void Unit::AttackTo(sf::Vector2i pos)
{
	Attack a = weapons.at(currentWeapon).GetAttack();
	if (AP >= a.ap && Distance(tile, pos)<=a.range) {
		Unit* target = GetUnit(Tile::tileRef[pos.x][pos.y].unit);
		if (target != nullptr) {
			AP -= a.ap;
			target->GetAttacked(a);
		}
	}
}

void Unit::GetAttacked(Attack a)
{
	if (a.successful) {
		HP -= a.damage.physical;
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

