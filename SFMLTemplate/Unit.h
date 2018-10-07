#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Resources.h"
#include "Constants.h"
#include "Tile.h"
#include "Attack.h"

class Unit
{
public:
	Unit(std::string name, int player=0);
	~Unit();
	static Unit* GetUnit(int id);
	void MoveTo(sf::Vector2i pos);
	void AttackTo(sf::Vector2i pos);
	void GetAttacked(Attack a);
	bool Dead() { return HP <= 0; };
	void EndOfTurn();
	std::string Print();
	int player;
	int id;
	sf::Vector2i tile;
	sf::Sprite sprite;
	static std::vector<Unit>* unit;
private:
	std::string name;
	static int unitCount;
	short maxAP;
	short AP;
	short maxHP;
	short HP;
};
