#pragma once
#include "Resources.h"

class Attack
{
public:
	Attack(std::string name);
	~Attack();
	short physical, fire, ice, lightning;
	short range, ap, mp;
	short roll;
	short successThreshold;
	bool successful;
};

