#include "Weapon.h"

Weapon::Weapon(std::string name, int level, int owner)
{
	this->owner = owner;
	this->level = level;
	this->name = name;
	baseDamage = level + 1;
	currentAttack = 0;
	inUse = false;
	std::vector<std::string> info = Resources::GetText("weapons/" + name);
	for (int i = 0; i < info.size(); i++) {
		std::pair<std::string, std::vector<std::string>> temp = Resources::KeyWithStrings(info.at(i));
		if (temp.first != "") {
			if (Resources::StrInVector(temp.first, Constants::attributes)) {
				requirment[temp.first] = std::stoi(temp.second.at(0)) + std::stoi(temp.second.at(1))*(level - 1);
			}
			if(temp.first=="attacks") {
				for (int j = 0; j < temp.second.size(); j++) {
					attacks.push_back(Attack(temp.second.at(j), baseDamage, owner));
				}
			}
		}
	}
}

Weapon::~Weapon()
{
}

std::string Weapon::Print(bool full, bool justName)
{
	std::string buffer = name + " lvl " + std::to_string(level);
	if (justName)
		return buffer;
	if (full) {
		for (int i = 0; i < attacks.size(); i++) {
			buffer += "\n" + attacks.at(i).name + " " +
				std::to_string(100 - attacks.at(i).successThreshold) + "% " +
				std::to_string(attacks.at(i).baseDmg);
		}
		return buffer;
	}
	return buffer + "\n" +
		attacks.at(currentAttack).name + " " +
		std::to_string(100-attacks.at(currentAttack).successThreshold)+"% " +
		std::to_string(attacks.at(currentAttack).baseDmg);
}

void Weapon::SwitchAttack(int i)
{
	if (i >= 0 && i < attacks.size())
		currentAttack = i;
	else {
		currentAttack++;
		if (currentAttack >= attacks.size())
			currentAttack = 0;
	}
}

Attack Weapon::GetAttack(int i)
{
	if (i < 0 || i >= attacks.size())
		i = currentAttack;
	attacks.at(i).Roll();
	return attacks.at(i);
}

bool Weapon::CanUse(std::map<std::string, int> attributes)
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
