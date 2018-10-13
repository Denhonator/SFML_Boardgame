#pragma once
#include "Resources.h"

class Attack
{
public:
	Attack(std::string name, short chancetohit, Damage d, short range, short ap, short mp);
	~Attack();
	void Roll();
	std::string name;
	Damage damage;
	short range, ap, mp;
	short roll;
	short successThreshold;
	bool crit, fail;
};

