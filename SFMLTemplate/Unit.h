#pragma once
#include "Resources.h"
#include "Constants.h"
#include "Tile.h"
#include "Weapon.h"
#include "Messages.h"
#include "Item.h"
#include "Equipment.h"

class Unit
{
public:
	Unit(std::string name, int player=0, sf::Vector2i pos=sf::Vector2i(0,0), std::string nick="");
	~Unit();
	static Unit* GetUnit(int id);
	static int Distance(sf::Vector2i a, sf::Vector2i b);
	static bool TransferWeapon(Unit* from, Unit* to, int index);
	void LoadFromFile(std::string path);
	bool MoveTo(sf::Vector2i pos, int ap = 3);
	bool MoveTowards(sf::Vector2i pos);
	bool LootFrom(sf::Vector2i pos);
	bool LootFrom(Unit* unit);
	bool AttackTo(sf::Vector2i pos);
	bool Push(sf::Vector2i pos);
	bool UseItem(sf::Vector2i pos, int i = -1);
	void GetAffected(Effect e);
	void GetAttacked(Attack a);
	void AddWeapon(std::string name, int level);
	void AddEquipment(std::string name, int level);
	void AddItem(std::string name, int level, int count);
	bool SwitchWeapon(int i = -1);
	bool Unequip(int i, int slot = 0, bool quiet = false);
	bool Equip(int i, int slot = 0, bool quiet = false);
	bool SwitchItem(int i = -1);
	Weapon* GetWeapon(int i = -1);
	void UpdateBonuses();
	bool Dead() { return HP <= 0; };
	void EndOfTurn();
	std::string Print(bool full = false, bool justName = false);
	std::string PrintLootable();
	void UpdateBars();
	int player;
	int id;
	sf::Vector2i tile;
	sf::Sprite sprite;
	sf::VertexArray bars;
	static std::vector<Unit>* unit;
	bool removed = false;
private:
	Unit* ChooseBodyFrom(sf::Vector2i pos);
	int pushedBy = -1;
	static int unitCount;
	std::string name, nick;
	Damage armor;
	int maxAP, AP;
	int maxHP, HP;
	int maxMP, MP;
	int XP;
	int tohit, tocast, criticalHit, criticalFail, luckBonus, charBonus;
	int currentWeapon, currentItem;
	std::vector<Weapon> weapons;
	std::vector<Item> items;
	std::vector<Equipment> equipment;
	std::map<std::string, Equipment*> equipSlot;
	std::map<std::string, int> attribute;
	std::map<std::string, int> attributeGain;
};
