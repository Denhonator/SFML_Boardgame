#include "Attack.h"

Attack::Attack(short attacker, std::string name, short chancetohit, Damage d, short range, short ap, short mp)
{
	this->name = name;
	this->range = range;
	this->attacker = attacker;
	damage = d;
	this->ap = ap;
	successThreshold = 100 - chancetohit;
	roll = -1;
	knockback = sf::Vector2i(0, 0);
}

Attack::~Attack()
{
}

void Attack::Roll()
{
	roll = Resources::Roll();
}
