#include "Attack.h"

Attack::Attack(std::string name, int baseDmg, int attacker)
{
	this->name = name;
	this->attacker = attacker;
	this->baseDmg = baseDmg;
	
	roll = -1;
	knockback = sf::Vector2i(0, 0);

	std::vector<std::string> info = Resources::GetText("attacks/" + name);

	for (unsigned int i = 0; i < info.size(); i++) {
		std::pair<std::string, std::vector<std::string>> temp = Resources::KeyWithStrings(info.at(i));
		if (temp.first != "") {
			if (temp.first == "HitChance") {
				successThreshold = 100 - std::stoi(temp.second.at(0));
			}
			else if (temp.first == "Range") {
				range = std::stoi(temp.second.at(0));
			}
			else if (temp.first == "AP") {
				ap = std::stoi(temp.second.at(0));
				if (ap == -1)
					ap = 5;		//Default AP cost
			}
			else if (temp.first == "MP") {
				mp = std::stoi(temp.second.at(0));
			}
			else if (temp.first == "Effects") {
				for (unsigned int j = 0; j < temp.second.size(); j++) {
					effects.push_back(* new Effect(temp.second.at(j), baseDmg - 1, attacker, name));
				}
			}
			else {
				damage[temp.first] = (baseDmg * std::stoi(temp.second.at(0))) / 100;
			}
		}
	}

}

Attack::~Attack()
{
}

void Attack::Roll()
{
	roll = Resources::Roll();
}
