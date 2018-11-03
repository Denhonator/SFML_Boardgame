#include "Effect.h"

Effect::Effect(std::string name, int level, int owner, std::string itemName)
{
	this->name = name;
	this->level = level;
	this->owner = owner;

	hp = 0;
	mp = 0;
	ap = 0;
	duration = 0;
	this->itemName = itemName;

	std::vector<std::string> info = Resources::GetText("effects/" + name);
	std::string buffer = "";
	for (int i = 0; i < info.size(); i++) {
		std::pair<std::string, std::vector<std::string>> temp = Resources::KeyWithStrings(info.at(i));
		if (temp.first == "hp") {
			hp = std::stoi(temp.second.at(0)) + std::stoi(temp.second.at(1))*(level - 1);
		}
		else if (temp.first == "mp") {
			mp = std::stoi(temp.second.at(0)) + std::stoi(temp.second.at(1))*(level - 1);
		}
		else if (temp.first == "ap") {
			ap = std::stoi(temp.second.at(0)) + std::stoi(temp.second.at(1))*(level - 1);
		}
		else if (temp.first == "duration") {
			duration = std::stoi(temp.second.at(0)) + std::stoi(temp.second.at(1))*(level - 1);
		}
	}
}

Effect::~Effect()
{
}
