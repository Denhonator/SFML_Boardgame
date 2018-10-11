#include "Weapon.h"

Weapon::Weapon(std::string name, short level)
{
	this->level = level;
	this->name = name;
	baseDamage = level + 1;
	currentAttack = 0;
	std::vector<std::string> info = Resources::GetText("weapons/" + name);
	std::string buffer = "";
	for (int i = 0; i < info.size(); i++) {
		std::pair<std::string, std::vector<int>> temp = Resources::KeyWithValues(info.at(i));
		if (temp.first != "") {
			short damage = (baseDamage * temp.second.at(1))/100;
			attacks.push_back(Attack(temp.first, temp.second.at(0), Damage{ damage,0,0,0 }, 1, 5, 0));
		}
	}
}

Weapon::~Weapon()
{
}

std::string Weapon::Print()
{
	return name + " lvl " + std::to_string(level) + "\n" +
		attacks.at(currentAttack).name + " " +
		std::to_string(100-attacks.at(currentAttack).successThreshold)+"% " +
		std::to_string(attacks.at(currentAttack).damage.physical) + "," +
		std::to_string(attacks.at(currentAttack).damage.fire) + "," +
		std::to_string(attacks.at(currentAttack).damage.ice) + "," +
		std::to_string(attacks.at(currentAttack).damage.lightning);
}

void Weapon::SwitchAttack(short i)
{
	if (i >= 0 && i < attacks.size())
		currentAttack = i;
	else {
		currentAttack++;
		if (currentAttack >= attacks.size())
			currentAttack = 0;
	}
}

Attack Weapon::GetAttack(short i)
{
	if (i < 0 || i >= attacks.size())
		i = currentAttack;
	attacks.at(i).Roll();
	return attacks.at(i);
}
