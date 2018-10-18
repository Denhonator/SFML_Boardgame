#pragma once
#include "Resources.h"
#include "Effect.h"

class Item
{
public:
	Item(std::string name, int level, int owner, int count = 1);
	~Item();
	std::vector<Effect>* Consume();
	std::string Print();
	int range;
private:
	std::string name;
	int owner;
	int value;
	int level;
	unsigned int count;
	bool consumable;
	std::vector<Effect> consumeEffects;
};

