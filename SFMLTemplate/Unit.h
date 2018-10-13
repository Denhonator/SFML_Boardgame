#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Resources.h"
#include "Constants.h"
#include "Tile.h"
#include "Weapon.h"
#include "Messages.h"

class Unit
{
public:
	Unit(std::string name, int player=0, sf::Vector2i pos=sf::Vector2i(0,0), std::string nick="");
	~Unit();
	static Unit* GetUnit(int id);
	void LoadFromFile(std::string path);
	void MoveTo(sf::Vector2i pos);
	void AttackTo(sf::Vector2i pos);
	void GetAttacked(Attack a);
	void AddWeapon(std::string name, short level);
	void SwitchWeapon(short i = -1);
	Weapon* GetWeapon(short i = -1);
	void UpdateBonuses();
	bool Dead() { return HP <= 0; };
	void EndOfTurn();
	std::string Print(bool full = false);
	void UpdateBars();
	int player;
	int id;
	sf::Vector2i tile;
	sf::Sprite sprite;
	sf::VertexArray bars;
	static std::vector<Unit>* unit;
private:
	static int unitCount;
	std::string name;
	std::string nick;
	short maxAP, AP;
	short maxHP, HP;
	short maxMP, MP;
	short XP;
	short tohit, tocast, criticalHit, criticalFail, luckBonus, charBonus;
	short currentWeapon;
	std::vector<Weapon> weapons;
	std::map<std::string, short> attribute;
	std::map<std::string, short> attributeGain;
};
