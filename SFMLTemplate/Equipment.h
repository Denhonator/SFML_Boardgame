#pragma once
#include "Resources.h"

class Equipment
{
public:
	Equipment(std::string name, int level, int owner);
	~Equipment();
	std::string Print(bool full = false, bool justName = false);
	bool CanUse(std::map<std::string, int> attributes);
	std::string GetType() {	return type; };
	bool inUse;
	int currentSlot;
	Damage armor;
private:
	std::string name, type;
	int owner;
	int level;
	int baseDefense;
	std::map<std::string, int> requirment;
};

