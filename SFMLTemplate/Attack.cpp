#include "Attack.h"

Attack::Attack(std::string name)
{
	range = 1;
	physical = 2;
	ap = 3;
	successThreshold = 50;
	roll = Resources::Roll();
	printf("%d/%d\n", roll, successThreshold);
	successful = roll >= successThreshold;
}

Attack::~Attack()
{
}
