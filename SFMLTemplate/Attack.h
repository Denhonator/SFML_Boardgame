#pragma once
#include "Resources.h"

class Attack
{
public:
	Attack(int attacker, std::string name, int chancetohit, Damage d, int range, int ap, int mp);
	~Attack();
	void Roll();
	std::string name;
	int attacker;
	Damage damage;
	int range, ap, mp;
	int roll;
	int successThreshold;
	bool crit, fail;
	sf::Vector2i knockback;
};

