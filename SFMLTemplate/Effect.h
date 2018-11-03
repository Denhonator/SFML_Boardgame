#pragma once
#include "Resources.h"

class Effect
{
public:
	Effect(std::string name, int level, int owner, std::string itemName="");
	~Effect();
	int hp, mp, ap;
	int owner;
	int duration;
	std::string name, itemName;
private:
	int level;
};

