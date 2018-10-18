#include "Item.h"

Item::Item(std::string name, int level, int owner, int count)
{
	this->name = name;
	this->level = level;
	this->owner = owner;
	this->count = count;
	consumable = false;
	range = 1;

	std::vector<std::string> info = Resources::GetText("items/" + name);
	std::string buffer = "";

	for (int i = 0; i < info.size(); i++) {
		std::pair<std::string, std::vector<std::string>> temp = Resources::KeyWithStrings(info.at(i));
		if (temp.first == "value") {
			value = std::stoi(temp.second.at(0)) + std::stoi(temp.second.at(1))*(level-1);
		}
		else if (temp.first == "type") {
			if (temp.second.at(0) == "consumable")
				consumable = true;
		}
		else if (temp.first == "effect") {
			if (consumable) {
				consumeEffects.push_back(Effect(temp.second.at(0), std::stoi(temp.second.at(1)) + std::stoi(temp.second.at(2))*(level-1), owner, name));
			}
		}
	}
}

Item::~Item()
{
}

std::vector<Effect>* Item::Consume()
{
	if (consumeEffects.size()&&count>=0) {
		count--;
		return &consumeEffects;
	}
	return nullptr;
}

std::string Item::Print()
{
	return name + " lvl " + std::to_string(level) + " (" + std::to_string(count) + ")";
}
