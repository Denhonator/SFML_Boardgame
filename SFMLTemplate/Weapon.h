#pragma once
#include "Attack.h"
class Weapon
{
public:
	Weapon(std::string name, short level);
	~Weapon();
	std::string Print();
	void SwitchAttack(short i = -1);
	Attack GetAttack(short i = -1);
private:
	std::string name;
	short level;
	short baseDamage;
	short currentAttack;
	std::vector<Attack> attacks;
	std::map<std::string, short> requirments;
};

