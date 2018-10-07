#include "Unit.h"

int Unit::unitCount = 0;
std::vector<Unit>* Unit::unit;

Unit::Unit(std::string name, int player)
{
	unitCount++;
	sprite.setTexture(*Resources::GetTexture("units/" + name + "/" + name));
	sprite.setScale(sf::Vector2f(Constants::tileSize / sprite.getLocalBounds().width, Constants::tileSize / sprite.getLocalBounds().height));
	this->player = player;
	this->name = name;
	maxAP = 8;
	maxHP = 20;
	AP = maxAP;
	HP = maxHP;
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
	Attack a = Attack("");
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
		HP -= a.physical;
	}
}

void Unit::EndOfTurn()
{
	AP = maxAP;
}

std::string Unit::Print()
{
	return name+" (ID: " + std::to_string(id)+")" +
		"\nAP: "+ std::to_string(AP)+"/"+std::to_string(maxAP) +
		"\nHP: "+ std::to_string(HP)+"/"+std::to_string(maxHP);
}

