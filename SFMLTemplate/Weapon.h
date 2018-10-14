#pragma once
#include "Attack.h"
#include "Constants.h"
class Weapon
{
public:
	Weapon(std::string name, short level, short owner);
	~Weapon();
	std::string Print();
	void SwitchAttack(short i = -1);
	Attack GetAttack(short i = -1);
	bool CanUse(std::map<std::string, short> attributes);
private:
	short owner;
	std::string name;
	short level;
	short baseDamage;
	short currentAttack;
	std::vector<Attack> attacks;
	std::map<std::string, short> requirment;
};

