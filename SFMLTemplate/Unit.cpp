#include "Unit.h"

int Unit::unitCount = 0;
std::vector<Unit>* Unit::unit;

Unit::Unit(std::string name, int player, sf::Vector2i pos, std::string nick)
{
	unitCount++;
	sprite.setTexture(*Resources::GetTexture("units/" + name + "/" + name));
	sprite.setScale(sf::Vector2f(Constants::tileSize / sprite.getLocalBounds().width, Constants::tileSize / sprite.getLocalBounds().height));
	this->player = player;
	this->name = name;
	if (nick == "")
		nick = name;
	this->nick = nick;
	XP = 0;
	id = unitCount;
	LoadFromFile("units/" + name + "/" + nick);
	if(weapons.size()==0)
		AddWeapon("self", 1);
	currentWeapon = 0;
	weapons.at(0).inUse = true;
	currentItem = 0;
	tile = pos;
	sprite.setPosition(tile.x*Constants::tileSize, tile.y*Constants::tileSize);
	Tile::tileRef[tile.x][tile.y].unit = id;
	bars = sf::VertexArray(sf::PrimitiveType::TriangleStrip, 18);
	UpdateBonuses();
	AP = maxAP;
	HP = maxHP;
	MP = maxMP;
	visionRange = 8;
	UpdateBars();
}

Unit::~Unit()
{
}

Unit * Unit::GetUnit(int id)
{
	for (int i = 0; i < unit->size(); i++) {
		if (unit->at(i).id == id)
			return &unit->at(i);
	}
	return nullptr;
}

void Unit::LoadFromFile(std::string path)
{
	std::vector<std::string> info = Resources::GetText(path);
	std::string buffer = "";
	for (int i = 0; i < info.size(); i++) {
		std::pair<std::string, std::vector<std::string>> temp = Resources::KeyWithStrings(info.at(i));
		if (Resources::StrInVector(temp.first, Constants::attributes)) {
			attribute[temp.first] = std::stoi(temp.second.at(0));
			attributeGain[temp.first] = std::stoi(temp.second.at(1));
		}
		else if (Resources::StrInVector(temp.first, Constants::equipSlots)) {
			AddEquipment(temp.second.at(0), std::stoi(temp.second.at(1)));
			char c = temp.first[temp.first.size() - 1];
			if (c > '9' || c < '0')
				c = '0';
			Equip(equipment.size() - 1, c-'0', true);
		}
		else if (temp.first == "weapon") {
			AddWeapon(temp.second.at(0), std::stoi(temp.second.at(1)));
		}
		else if (temp.first == "equipment") {
			AddEquipment(temp.second.at(0), std::stoi(temp.second.at(1)));
		}
		else if (temp.first == "item") {
			AddItem(temp.second.at(0), std::stoi(temp.second.at(1)), std::stoi(temp.second.at(2)));
		}
		else if (temp.first == "xp") {
			XP = std::stoi(temp.second.at(0));
		}
	}
}

void Unit::AddWeapon(std::string name, int level)
{
	weapons.push_back(Weapon(name, level, id));
}

void Unit::AddEquipment(std::string name, int level)
{
	equipment.push_back(Equipment(name, level, id));
}

void Unit::AddItem(std::string name, int level, int count)
{
	items.push_back(Item(name, level, id, count));
}

bool Unit::SwitchWeapon(int i)
{
	if (i == currentWeapon)
		return false;
	if (AP < 2 || weapons.size()==1) {
		if (AP < 2)
			Messages::Notice("Not enough AP to switch weapon");
		else
			Messages::Notice("You only have one weapon");
		return false;
	}
	weapons.at(currentWeapon).inUse = false;
	if (i >= 0 && i < weapons.size())
		currentWeapon = i;
	else {
		currentWeapon++;
		if (currentWeapon >= weapons.size())
			currentWeapon = 0;
	}
	AP -= 2;
	weapons.at(currentWeapon).inUse = true;
	Messages::Add(nick + " equipped " + weapons.at(currentWeapon).Print(false, true));
	UpdateBars();
	return true;
}

bool Unit::Unequip(int i, int slot, bool quiet)
{
	if (!equipment.at(i).inUse||(AP<2&&!quiet))
		return false;
	AP -= 2;
	UpdateBars();
	equipment.at(i).inUse = false;
	equipSlot.erase(equipment.at(i).GetType() + std::to_string(equipment.at(i).currentSlot));
	if(!quiet)
		Messages::Add(nick + " unequipped " + equipment.at(i).Print(false, true));
	return true;
}

bool Unit::Equip(int i, int slot, bool quiet)
{
	if (Unequip(i,slot,quiet)) {
		return true;
	}
	if (i >= 0 && i < equipment.size()) {
		if (!quiet&&!equipment.at(i).CanUse(attribute)) {
			Messages::Notice("You don't meet the requirments");
			return false;
		}
		if (AP < 2 && !quiet) {
			Messages::Notice("Not enough AP");
			return false;
		}
		AP -= 2;
		std::string s = equipment.at(i).GetType();
		if (slot > 0 && slot < 10)
			s += std::to_string(slot);

		if (equipSlot.count(s)) {	//If slot taken, unequip that
			equipSlot[s]->inUse = false;
			equipSlot.erase(s);
		}
		equipment.at(i).inUse = true;
		equipment.at(i).currentSlot = slot;
		equipSlot[s] = &equipment.at(i);
		if(!quiet)
			Messages::Add(nick + " equipped " + equipment.at(i).Print(false, true));
		return true;
	}
	return false;
}

bool Unit::SwitchItem(int i)
{
	if (i < 0)
		i = currentItem + 1;
	if (i >= items.size())
		i = 0;
	currentItem = i;
	return items.size();
}

Weapon * Unit::GetWeapon(int i)
{
	if (i<0 || i>weapons.size()) {
		i = currentWeapon;
	}
	return &weapons.at(i);
}

bool Unit::LevelUp(std::string a)
{
	if (attribute.count(a)) {
		int req = attribute[a] * attributeGain[a];
		if (XP >= req) {
			XP -= req;
			attribute[a]++;
			UpdateBonuses();
			UpdateBars();
			return true;
		}
	}
	return false;
}

void Unit::UpdateBonuses()
{
	maxAP = 8 + attribute["dexterity"];
	maxHP = 3 * attribute["vitality"];
	maxMP = 3 * attribute["intelligence"];
	tohit = -24 + attribute["dexterity"] * 3;
	tocast = -24 + attribute["willpower"] * 3;
	criticalHit = 12 - attribute["perception"] * 2;
	criticalFail = 10 - attribute["perception"];
	luckBonus = -8 + attribute["luck"];
	charBonus = -10 + attribute["charisma"] * 4;

	armor = { 0,0,0,0 };
	for (int i = 0; i < equipment.size(); i++) {
		if (equipment.at(i).inUse) {
			armor.physical += equipment.at(i).armor.physical;
			armor.fire += equipment.at(i).armor.fire;
			armor.ice += equipment.at(i).armor.ice;
			armor.lightning += equipment.at(i).armor.lightning;
		}
	}
}

int Unit::Distance(sf::Vector2i a, sf::Vector2i b) {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

bool Unit::TransferWeapon(Unit * from, Unit * to, int index)
{
	if (from->weapons.size() > index&&index >= 0) {
		from->weapons.at(index).inUse = false;
		to->weapons.push_back(from->weapons.at(index));
		Messages::Add(to->nick + " looted " + from->weapons.at(index).Print(false, true) + " from " + from->nick);
		from->weapons.erase(from->weapons.begin() + index);
		return true;
	}
	return false;
}

bool Unit::MoveTo(sf::Vector2i pos, int ap)
{
	pos = sf::Vector2i(std::min(std::max(pos.x, 0), Constants::boardSize - 1), std::min(std::max(pos.y, 0), Constants::boardSize - 1));
	if (Tile::tileRef[pos.x][pos.y].sprite != "0" && Tile::tileRef[pos.x][pos.y].unit==-1
		&& (Tile::tileRef[pos.x][tile.y].sprite != "0" || Tile::tileRef[tile.x][pos.y].sprite != "0")) {
		if (std::abs(tile.x - pos.x) < 2 && std::abs(tile.y - pos.y) < 2) {
			if (Distance(pos, tile) == 2) {
				if (AP >= ap*1.3f)
					AP -= ap*1.3f;
				else
					return false;
			}
			else if (Distance(pos, tile) == 1) {
				if (AP >= ap)
					AP -= ap;
				else
					return false;
			}
			else
				return false;
			sprite.setPosition(sf::Vector2f(pos.x*Constants::tileSize, pos.y*Constants::tileSize));
			Tile::tileRef[tile.x][tile.y].unit = -1;
			tile = pos;
			Tile::tileRef[tile.x][tile.y].unit = id;
			UpdateBars();
			return true;
		}
	}
	return false;
}

bool Unit::MoveTowards(sf::Vector2i pos)
{
	sf::Vector2i dif = sf::Vector2i(pos.x-tile.x, pos.y-tile.y);
	if (dif.x > 0)
		dif.x = 1;
	if (dif.x < 0)
		dif.x = -1;
	if (dif.y > 0)
		dif.y = 1;
	if (dif.y < 0)
		dif.y = -1;
	std::vector<sf::Vector2i> voffs = Resources::Voffs();
	for (int i = 0; i < voffs.size();i++) {
		if (MoveTo(tile + dif + voffs.at(i)))
			return true;
	}
	for (int i = 0; i < voffs.size(); i++) {
		if (Push(tile + dif + voffs.at(i)))
			return true;
	}
	return false;
}

bool Unit::MovePath()
{
	if (currentPath.size()) {
		for (unsigned int i = 0; i < currentPath.size() - 1; i++) {
			if (currentPath.at(i) == tile) {
				if (!MoveTo(currentPath.at(i + 1))) {
					return false;
				}
				return true;
			}
		}
	}
	return false;
}

Unit* Unit::ChooseBodyFrom(sf::Vector2i pos)
{
	Unit* u;
	std::string buffer = "Loot from:\n";
	std::vector<int> units = Tile::tileRef[pos.x][pos.y].otherUnits;
	if (units.size() == 0)
		return nullptr;

	for (int i = 0; i < units.size(); i++) {
		u = GetUnit(units.at(i));
		if (u != nullptr&&i != units.size() - 1)
			buffer += std::to_string(i) + ": " + u->Print(false, true) + "\n";
		else if (u != nullptr)
			buffer += std::to_string(i) + ": " + u->Print(false, true);
	}
	char key = Messages::Prompt(buffer);
	int sel = key - '0';
	if (sel >= 0 && sel < units.size()) {
		return GetUnit(units.at(sel));
	}
	return nullptr;
}

bool Unit::LootFrom(sf::Vector2i pos)
{
	if (Distance(pos, tile) < 2 && AP>=2) {
		Unit* u = ChooseBodyFrom(pos);
		if (u != nullptr) {
			AP -= 2;
			UpdateBars();
			while (LootFrom(u)) {
				;
			}
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

bool Unit::LootFrom(Unit * unit)
{
	std::string buffer = "";
	if (unit->weapons.size() == 0)
		return false;

	for (int i = 0; i < unit->weapons.size(); i++) {
		buffer += std::to_string(i) + ": " + unit->weapons.at(i).Print(true);
		if (i != weapons.size() - 1)
			buffer += "\n";
	}
	char key = Messages::Prompt(buffer);
	int sel = key - '0';
	if (sel >= 0 && sel < unit->weapons.size()) {
		return TransferWeapon(unit, this, sel);
	}
	else {
		return LootFrom(unit->tile);
	}
}

bool Unit::AttackTo(sf::Vector2i pos, bool dry, sf::Sprite* aSprite)
{
	if (weapons.at(currentWeapon).CanUse(attribute)) {
		Attack a = weapons.at(currentWeapon).GetAttack();
		a.fail = a.roll <= 5 + criticalFail;
		a.crit = a.roll >= 95 + criticalHit;
		if (AP >= a.ap && Distance(tile, pos) <= a.range) {
			Unit* target = GetUnit(Tile::tileRef[pos.x][pos.y].unit);
			if (target != nullptr) {
				if (dry)
					return true;

				if(target->player == player && player!=0){
					a.roll += charBonus;
					Messages::Prompt("Attack friendly unit " + target->nick + "?");
					while (1) {
						sf::sleep(sf::milliseconds(2));
						char key = Messages::WaitInput();
						key = std::tolower(key, std::locale());
						if (key == 'y')
							break;
						else if (key != 0)
							return false;
					}
				}

				a.roll += tohit;
				AP -= a.ap;
				if (aSprite != nullptr) {
					sf::Texture * t = Resources::GetTexture("attacks/" + GetWeapon()->GetAttack().name);
					aSprite->setTexture(t != nullptr ? *t : *Resources::GetTexture("attacks/default"));	//Set texture to attack specific or default if missing
					sf::Vector2f offset = sf::Vector2f(aSprite->getGlobalBounds().width / 2, aSprite->getGlobalBounds().height / 2);
					aSprite->setPosition(sprite.getPosition()+offset);
					sf::Vector2f attackDir = target->sprite.getPosition()+offset - aSprite->getPosition();
					for (unsigned int i = 0; i < 30; i++) {
						aSprite->move(attackDir.x / 30, attackDir.y / 30);
						sf::sleep(sf::milliseconds(16));
					}
					aSprite->setPosition(0, 0);
				}
				Messages::Add(name + " rolls " + std::to_string(a.roll) + "/" + std::to_string(a.successThreshold) + " on " + a.name + " against " + target->nick);
				if(a.roll>=a.successThreshold&&!a.fail)
					target->GetAttacked(a);
				else {
					Resources::PlayWav("woosh");
					if (a.fail)
						Messages::Add("Critical failure");
				}
				UpdateBars();
				return true;
			}
			else {
				Messages::Notice("Invalid target");
			}
		}
	}
	else {
		Messages::Notice("Unable to use current weapon");
	}
	return false;
}

bool Unit::Push(sf::Vector2i pos)
{
	if (pos == tile || pos.x<0 || pos.y<0 ||pos.x>Constants::boardSize||pos.y>Constants::boardSize)
		return false;
	Attack a = Attack(id, "push", 95, Damage{ 0,0,0,0 }, 1, 4, 0);
	a.Roll();
	a.knockback = pos - tile;
	if (AP >= a.ap) {
		Unit* target = GetUnit(Tile::tileRef[pos.x][pos.y].unit);
		if (target != nullptr && target->player==player && target->id!=pushedBy) {
			AP -= a.ap;
			if(a.roll>=a.successThreshold)
				target->GetAttacked(a);
			return true;
		}
	}
	return false;
}

bool Unit::UseItem(sf::Vector2i pos, int i)
{
	if (i < 0) {
		i = currentItem;
	}
	if (i >= 0 && i < items.size() && items.at(i).range>=Distance(pos,tile) && AP >= 5) {
		Unit* u;
		u = pos == tile ? this : GetUnit(Tile::tileRef[pos.x][pos.y].unit);
		if (u == nullptr)
			return false;
		std::vector<Effect>* effects = items.at(i).Consume();
		for (int j = 0; j < effects->size(); j++) {
			u->GetAffected(effects->at(j));
		}
		AP -= 5;
		UpdateBars();
		return true;
	}
	return false;
}

void Unit::GetAffected(Effect e)
{
	Unit* u = GetUnit(e.owner);
	Messages::Add((u!=nullptr ? u->nick : "Someone") + " used " + e.itemName + " on " + nick);
	HP = std::min(maxHP, HP + e.hp);
	MP = std::min(maxMP, MP + e.mp);
	AP = std::min(maxAP, AP + e.ap);
	UpdateBars();
}

void Unit::GetAttacked(Attack a)
{
	int mult = a.crit ? 2 : 1;
	int temp = HP;
	HP -= a.damage.physical * mult;
	HP -= a.damage.fire * mult;
	HP -= a.damage.ice * mult;
	HP -= a.damage.lightning * mult;
	if (a.knockback.x || a.knockback.y) {
		MoveTo(tile + a.knockback, 0);
		pushedBy = a.attacker;
	}
	if (HP < temp) {
		Resources::PlayWav("hit");
		if (a.crit)
			Messages::Add("Critical hit!");
		Messages::Add(name + " takes " + std::to_string(a.damage.physical) + " damage");
	}
	UpdateBars();
}

void Unit::EndOfTurn()
{
	AP = maxAP;
	UpdateBars();
}

std::string Unit::Print(bool full, bool justName)
{
	if (justName)
		return nick;
	std::string buffer = nick + " (ID: " + std::to_string(id) + ")" +
		"\nHP: " + std::to_string(HP) + "/" + std::to_string(maxHP) +
		"\nAP: " + std::to_string(AP) + "/" + std::to_string(maxAP) +
		"\nMP: " + std::to_string(MP) + "/" + std::to_string(maxMP);

	if(full) {
		buffer += "\n" + weapons.at(currentWeapon).Print();
		if(items.size())
			buffer += "\nItem: " + items.at(currentItem).Print();
		buffer += "\nArmor: " + std::to_string(armor.physical) + "," +
			std::to_string(armor.fire) + "," +
			std::to_string(armor.ice) + "," +
			std::to_string(armor.lightning) +
			"\nTo hit bonus: " + std::to_string(tohit) +
			"\nCrit chance: " + std::to_string(100 - (95 + criticalHit)) + "%" +
			"\nFail chance: " + std::to_string(5 + criticalFail) + "%";
	}
	return buffer;
}

std::string Unit::PrintStats(sf::String arg)
{
	if (arg.find("XP") != sf::String::InvalidPos) {
		return std::to_string(XP);
	}
	return "";
}

std::string Unit::PrintLootable()
{
	return std::string();
}

int Unit::GetAttribute(std::string s)
{
	if (attribute.count(s)) {
		return attribute[s];
	}
	return 0;
}

int Unit::GetAtrRequirment(std::string s)
{
	if (attribute.count(s)) {
		return attribute[s] * attributeGain[s];
	}
	return -1;
}

void Unit::UpdateBars()
{
	float width = Constants::tileSize / 40;
	float heigth = Constants::tileSize*std::pow(maxHP/30.0f,1.0f);
	sf::Vector2f offset = sf::Vector2f(0, Constants::tileSize / 2 - heigth / 2);
	float line = heigth * (1.0f-((float)HP / (float)maxHP));
	bars[0].position = sprite.getPosition() + offset;
	bars[1].position = sprite.getPosition() + offset + sf::Vector2f(width, 0);
	bars[2].position = sprite.getPosition() + offset + sf::Vector2f(0, line);
	bars[3].position = sprite.getPosition() + offset + sf::Vector2f(width, line);
	bars[4].position = sprite.getPosition() + offset + sf::Vector2f(0, line);
	bars[5].position = sprite.getPosition() + offset + sf::Vector2f(width, line);
	bars[6].position = sprite.getPosition() + offset + sf::Vector2f(0, heigth);
	bars[7].position = sprite.getPosition() + offset + sf::Vector2f(width, heigth);
	heigth = Constants::tileSize*std::pow(maxAP / 25.0f, 1.0f);
	offset = sf::Vector2f(width, Constants::tileSize / 2 - heigth / 2);
	line = heigth * (1.0f - ((float)AP / (float)maxAP));
	bars[8].position = bars[7].position;
	bars[9].position = sprite.getPosition() + offset;
	bars[10].position = sprite.getPosition() + offset;
	bars[11].position = sprite.getPosition() + offset + sf::Vector2f(width, 0);
	bars[12].position = sprite.getPosition() + offset + sf::Vector2f(0, line);
	bars[13].position = sprite.getPosition() + offset + sf::Vector2f(width, line);
	bars[14].position = sprite.getPosition() + offset + sf::Vector2f(0, line);
	bars[15].position = sprite.getPosition() + offset + sf::Vector2f(width, line);
	bars[16].position = sprite.getPosition() + offset + sf::Vector2f(0, heigth);
	bars[17].position = sprite.getPosition() + offset + sf::Vector2f(width, heigth);

	for (int i = 0; i < 8; i++) {
		bars[i].color = sf::Color(i<4?0:200, 0, 0, 255);
	}
	for (int i = 8; i < 10; i++) {
		bars[i].color = sf::Color(0, 0, 0, 0);
	}
	for (int i = 10; i < 18; i++) {
		int mult = i < 14 ? 0 : 1;
		bars[i].color = sf::Color(255*mult, 255*mult, 200*mult, 255);
	}
}

