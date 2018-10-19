#pragma once
#include "Attack.h"
#include "Constants.h"

class Weapon
{
public:
	Weapon(std::string name, int level, int owner);
	~Weapon();
	std::string Print(bool full = false, bool justName = false);
	void SwitchAttack(int i = -1);
	Attack GetAttack(int i = -1);
	bool CanUse(std::map<std::string, int> attributes);
private:
	int owner;
	std::string name;
	int level;
	int baseDamage;
	int currentAttack;
	std::vector<Attack> attacks;
	std::map<std::string, int> requirment;
};

