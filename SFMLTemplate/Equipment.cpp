#include "Equipment.h"

Equipment::Equipment(std::string name, int level, int owner)
{
	this->owner = owner;
	this->level = level;
	this->name = name;
	
	inUse = false;
	currentSlot = 0;

	std::vector<std::string> info = Resources::GetText("equipment/" + name);
	std::string buffer = "";
	for (int i = 0; i < info.size(); i++) {
		std::pair<std::string, std::vector<std::string>> temp = Resources::KeyWithStrings(info.at(i));
		if (temp.first != "") {
			if (Resources::StrInVector(temp.first, Constants::attributes)) {
				requirment[temp.first] = std::stoi(temp.second.at(0)) + std::stoi(temp.second.at(1)) * (level - 1);
			}
			else if (temp.first == "type") {
				type = temp.second.at(0);
			}
			else if (temp.first=="baseDefense") {
				baseDefense = std::stoi(temp.second.at(0)) + (std::stoi(temp.second.at(1)) * (level - 1))/100;
			}
		}
	}

	armor = { baseDefense,0,0,0 };
}

Equipment::~Equipment()
{
}

std::string Equipment::Print(bool full, bool justName)
{
	std::string buffer = name + " lvl " + std::to_string(level);
	if (justName)
		return buffer;
	return buffer +	std::to_string(baseDefense);
}

bool Equipment::CanUse(std::map<std::string, int> attributes)
{
	for (int i = 0; i < Constants::attributes.size(); i++) {
		if (requirment.count(Constants::attributes.at(i))) {
			if (attributes[Constants::attributes.at(i)] < requirment[Constants::attributes.at(i)]) {
				return false;
			}
		}
	}
	return true;
}
