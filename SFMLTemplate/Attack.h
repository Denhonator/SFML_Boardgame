#pragma once
#include "Resources.h"
#include "Effect.h"

class Attack
{
public:
	Attack(std::string name, int baseDmg, int attacker);
	~Attack();
	void Roll();
	std::string name;
	int attacker;
	int baseDmg;
	int range, ap, mp;
	int roll;
	int successThreshold;
	bool crit, fail;
	sf::Vector2i knockback;
	std::vector<Effect> effects;
	std::map<std::string, int> damage;
};

