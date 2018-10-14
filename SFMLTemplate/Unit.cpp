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
	LoadFromFile("units/" + name + "/" + nick);
	if(weapons.size()==0)
		AddWeapon("self", 1);
	currentWeapon = 0;
	id = unitCount;
	tile = pos;
	sprite.setPosition(tile.x*Constants::tileSize, tile.y*Constants::tileSize);
	Tile::tileRef[tile.x][tile.y].unit = id;
	bars = sf::VertexArray(sf::PrimitiveType::TriangleStrip, 18);
	UpdateBonuses();
	AP = maxAP;
	HP = maxHP;
	MP = maxMP;
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
		else if (temp.first == "weapon") {
			AddWeapon(temp.second.at(0), std::stoi(temp.second.at(1)));
		}
		else if (temp.first == "xp") {
			XP = std::stoi(temp.second.at(0));
		}
	}
}

void Unit::AddWeapon(std::string name, short level)
{
	weapons.push_back(Weapon(name, level, id));
}

void Unit::SwitchWeapon(short i)
{
	if (AP < 2 || weapons.size()==1) {
		if (AP < 2)
			Messages::Notice("Not enough AP to switch weapon");
		else
			Messages::Notice("You only have one weapon");
		return;
	}
	if (i >= 0 && i < weapons.size())
		currentWeapon = i;
	else {
		currentWeapon++;
		if (currentWeapon >= weapons.size())
			currentWeapon = 0;
	}
	AP -= 2;
}

Weapon * Unit::GetWeapon(short i)
{
	if (i<0 || i>weapons.size()) {
		i = currentWeapon;
	}
	return &weapons.at(i);
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
}

short Unit::Distance(sf::Vector2i a, sf::Vector2i b) {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

bool Unit::MoveTo(sf::Vector2i pos, short ap)
{
	pos = sf::Vector2i(std::min(std::max(pos.x, 0), Constants::boardSize - 1), std::min(std::max(pos.y, 0), Constants::boardSize - 1));
	if (Tile::tileRef[pos.x][pos.y].sprite != "0" && Tile::tileRef[pos.x][pos.y].unit==-1) {
		if (std::abs(tile.x - pos.x) < 2 && std::abs(tile.y - pos.y) < 2) {
			if (Distance(pos, tile) == 2) {
				if (AP >= ap*1.5f)
					AP -= ap*1.5f;
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
	for (short i = 0; i < voffs.size();i++) {
		if (MoveTo(tile + dif + voffs.at(i)))
			return true;
	}
	for (short i = 0; i < voffs.size(); i++) {
		if (Push(tile + dif + voffs.at(i)))
			return true;
	}
	return false;
}

bool Unit::AttackTo(sf::Vector2i pos)
{
	if (weapons.at(currentWeapon).CanUse(attribute)) {
		Attack a = weapons.at(currentWeapon).GetAttack();
		a.fail = a.roll <= 5 + criticalFail;
		a.crit = a.roll >= 95 + criticalHit;
		if (AP >= a.ap && Distance(tile, pos) <= a.range) {
			Unit* target = GetUnit(Tile::tileRef[pos.x][pos.y].unit);
			if (target != nullptr) {

				if(target->player == player && player!=0){
					a.roll += charBonus;
					Messages::Prompt("Attack friendly unit " + target->nick + "?");
					while (1) {
						sf::sleep(sf::milliseconds(2));
						sf::Keyboard::Key key = Messages::WaitInput();
						if (key == sf::Keyboard::Key::Y)
							break;
						else if (key != sf::Keyboard::Key::Unknown)
							return false;
					}
				}

				a.roll += tohit;
				AP -= a.ap;
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

void Unit::GetAttacked(Attack a)
{
	short mult = a.crit ? 2 : 1;
	short temp = HP;
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

std::string Unit::Print(bool full)
{
	if (full) {
		return nick + " (ID: " + std::to_string(id) + ")" +
			"\nHP: " + std::to_string(HP) + "/" + std::to_string(maxHP) +
			"\nAP: " + std::to_string(AP) + "/" + std::to_string(maxAP) +
			"\nMP: " + std::to_string(MP) + "/" + std::to_string(maxMP) +
			"\n" + weapons.at(currentWeapon).Print() +
			"\nTo hit bonus: " + std::to_string(tohit) +
			"\nCrit chance: " + std::to_string(100-(95+criticalHit)) + "%" +
			"\nFail chance: " + std::to_string(5+criticalFail) + "%";
	}
	return nick + " (ID: " + std::to_string(id) + ")" +
		"\nHP: " + std::to_string(HP) + "/" + std::to_string(maxHP) +
		"\nAP: " + std::to_string(AP) + "/" + std::to_string(maxAP) +
		"\nMP: " + std::to_string(MP) + "/" + std::to_string(maxMP);
}

void Unit::UpdateBars()
{
	float width = Constants::tileSize / 40;
	float heigth = Constants::tileSize*std::pow(maxHP/30.0f,1.0f);
	sf::Vector2f offset = sf::Vector2f(4, Constants::tileSize / 2 - heigth / 2);
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
	offset = sf::Vector2f(5+width, Constants::tileSize / 2 - heigth / 2);
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

	for (short i = 0; i < 8; i++) {
		bars[i].color = sf::Color(i<4?0:200, 0, 0, 255);
	}
	for (short i = 8; i < 10; i++) {
		bars[i].color = sf::Color(0, 0, 0, 0);
	}
	for (short i = 10; i < 18; i++) {
		short mult = i < 14 ? 0 : 1;
		bars[i].color = sf::Color(255*mult, 255*mult, 200*mult, 255);
	}
}

