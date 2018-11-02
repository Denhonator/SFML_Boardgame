#include "Scene.h"

Scene::Scene()
{
	srand(time(NULL));
	board.Randomize();
	for (int i = 0; i < board.boardSize.x; i++) {
		for (int j = 0; j < board.boardSize.y; j++) {
			if (i + j > 7 && board.GetTile(i, j).sprite == "grass"&&board.GetTile(i, j).unit == -1 && rand() % 101 > 95) {
				int r = rand() % 101;
				if(r<30)
					AddUnit(&Unit("cave crab", 0, sf::Vector2i(i, j)));
				else if(r<60)
					AddUnit(&Unit("slime", 0, sf::Vector2i(i, j)));
				else if(r<90)
					AddUnit(&Unit("snake bat", 0, sf::Vector2i(i, j)));
				else
					AddUnit(&Unit("snake bat", 0, sf::Vector2i(i, j)));
			}
		}
	}
	const sf::Texture& bg = board.GetTexture(true)->getTexture();
	AddTile(&sf::Sprite(bg));
	boardUi.push_back(sf::Sprite(*Resources::GetTexture("ui/outline")));
	boardUi.at(0).setScale(Constants::tileSize / boardUi.at(0).getLocalBounds().width, Constants::tileSize / boardUi.at(0).getLocalBounds().height);
	for (int i = 0; i < 10; i++) {
		texts.push_back(sf::Text("", *Resources::GetFont("default.ttf")));
		texts.at(i).setPosition(sf::Vector2f(i>4 ? 2100 : 0, i%5 * 200));
	}
	mouseTile = sf::Vector2i(0, 0);
	AddUnit(&Unit("swordguy", 1, mouseTile,"denho"));
	currentPlayer = 1;
	currentUnit = -1;
	aiUnit = -1;
	currentAction = "";
	players = { 1 };
	menu = Menu(-1, -1);
	boardUiV.setPrimitiveType(sf::PrimitiveType::LineStrip);
	UpdateState();

	AI = std::thread(&Scene::AITurn, this);
}

Scene::~Scene()
{
	running = false;
	if (AI.joinable())
		AI.join();
}

void Scene::CheckAICombat()
{
	AIInCombat.clear();
	for (int i = 0; i < units.size(); i++) {
		if (!units.at(i).Dead() && units.at(i).player == 0) {
			if(AIFindTarget(i)!=-1)
				AIInCombat.push_back(i);
		}
	}
}

void Scene::AITurn()
{
	std::vector<sf::Vector2i> voffs;
	int range = 1;
	while (running) {
		while (aiUnit > -1 && aiUnit < units.size()) {
			range = units.at(aiUnit).GetWeapon()->GetAttack().range;
			voffs = Resources::Voffs(true, range);
			if (units.at(aiUnit).player == 0 && !units.at(aiUnit).Dead()) {
				if (units.at(aiUnit).target >= 0) {
					int dist = 999;
					int index = units.at(aiUnit).target;
					std::vector<sf::Vector2i> path;
					int shortest = -1;
					if (Unit::Distance(units.at(aiUnit).tile, units.at(index).tile) > range) {
						for (unsigned int i = 1; i < voffs.size(); i++) {	//Find fastest path to a tile in range of current attack
							if (board.CheckLOS((units.at(index).tile + voffs.at(i)).x, (units.at(index).tile + voffs.at(i)).y, units.at(index).tile.x, units.at(index).tile.y)) {	//Only choose a tile with line of sight to target
								path = board.FindPath(units.at(aiUnit).tile, units.at(index).tile + voffs.at(i));
								if (path.size() && path.size() < dist && path.size() < 12) {
									units.at(aiUnit).currentPath = path;
									dist = path.size();
								}
							}
						}
					}

					boardUiV.clear();	//pathing debug
					path = units.at(aiUnit).currentPath;
					for (unsigned int i = 0; i < path.size(); i++) {
						boardUiV.append(sf::Vertex(sf::Vector2f(path.at(i).x*Constants::tileSize + Constants::tileSize / 2, path.at(i).y*Constants::tileSize + Constants::tileSize / 2), sf::Color(255, 50, 50, 255)));
					}

					if (units.at(aiUnit).currentPath.size()) {
						for (int i = 0; i < 5; i++) {
							if (units.at(aiUnit).MovePath())
								sf::sleep(sf::milliseconds(300));
							if (Unit::Distance(units.at(aiUnit).tile, units.at(index).tile) == range) {
								break;
							}
						}
					}

					while (units.at(aiUnit).AttackTo(units.at(index).tile)) {
						sf::sleep(sf::milliseconds(300));
						if (units.at(index).Dead()) {
							index = AIFindTarget(aiUnit);
							if (index == -1) {
								break;
							}
						}
					}
				}
				else if (units.at(aiUnit).currentPath.size()) {
					while (units.at(aiUnit).MovePath())
						sf::sleep(sf::milliseconds(300));
					units.at(aiUnit).currentPath.clear();
				}
			}
			aiUnit++;
			if (aiUnit == units.size()) {
				aiUnit = -2;
				break;
			}
		}
		sf::sleep(sf::milliseconds(10));
	}
}

int Scene::AIFindTarget(int unitIndex)
{
	int dist = 100;
	int index = -1;
	for (int i = 0; i < units.size(); i++) {
		if (units.at(i).player == 0)
			continue;
		int newDist = Unit::Distance(units.at(unitIndex).tile, units.at(i).tile);
		if (newDist < dist && !units.at(i).Dead() && board.CheckLOS(units.at(unitIndex).tile.x,units.at(unitIndex).tile.y,units.at(i).tile.x,units.at(i).tile.y)) {
			dist = newDist;
			index = i;
		}
	}
	if (dist >= 8)
		index = -1;
	units.at(unitIndex).target = index;
	if (units.at(unitIndex).currentPath.size())
		return -2;
	return index;
}

int Scene::NextFromList(int to, std::vector<int> list)
{
	for (int i = 0; i < list.size(); i++) {
		if (to == list.at(i)) {
			if (i == list.size() - 1)
				return list.at(0);
			else
				return list.at(i + 1);
		}
	}
}

void Scene::SetCombat(bool val)
{
	if (combat && !val) {
		Messages::Add("Combat ended");
	}
	else if (!combat && val) {
		Messages::Add("Combat started");
	}
	combat = val;
}

void Scene::Update()
{
	if (currentPlayer == 0 && players.size()>1) {
		if (aiUnit == -2) {
			aiUnit = -1;
			EndTurn();
		}
		else if (aiUnit == -1) {
			int prev = AIInCombat.size();
			CheckAICombat();
			if (AIInCombat.size()) {
				SetCombat(true);
				if (prev) {
					aiUnit = 0;
				}
				else
					currentPlayer = NextFromList(currentPlayer, players);
			}
			else {
				SetCombat(false);
				currentPlayer = NextFromList(currentPlayer, players);
			}
		}
	}

	if (board.refresh) {
		for (unsigned int x = 0; x < board.boardSize.x; x++) {
			for (unsigned int y = 0; y < board.boardSize.y; y++) {
				Tile::tileRef[x][y].seen = false;
			}
		}
		for (unsigned int i = 0; i < playerUnits.size(); i++) {
			Unit* u = FindUnit(playerUnits.at(i));
			if (u != nullptr) {
				for (unsigned int x = 0; x < board.boardSize.x; x++) {
					for (unsigned int y = 0; y < board.boardSize.y; y++) {
						int dist = std::abs((int)x - u->tile.x) + std::abs((int)y - u->tile.y);
						if(dist <= u->GetVisionRange() && (dist==u->GetVisionRange()||dist==7||x==0||y==0||x==board.boardSize.x-1||y==board.boardSize.y-1))
							board.CheckLOS(u->tile.x, u->tile.y, x, y, true);
					}
				}
			}
		}
		tiles.at(0).setTexture(board.GetTexture(board.refresh)->getTexture());
		board.refresh = false;
	}
	if (update)
		UpdateState();
}

void Scene::AddTile(sf::Sprite * spr)
{
	tiles.push_back(*spr);
}

void Scene::AddUnit(Unit * unit)
{
	units.push_back(*unit);
	if (unit->player > 0) {
		playerUnits.push_back(unit->id);
	}
	bool found = false;
	for (int i = 0; i < players.size(); i++) {
		if (unit->player == players.at(i))
			found = true;
	}
	if (!found)
		players.push_back(unit->player);
	Unit::unit = &units;
}

void Scene::RemoveUnit(unsigned int i)
{
	if (currentUnit == units.at(i).id) {
		currentUnit = -1;
		currentAction = "";
	}
	Tile::tileRef[units.at(i).tile.x][units.at(i).tile.y].unit = -1;
	Tile::tileRef[units.at(i).tile.x][units.at(i).tile.y].otherUnits.push_back(units.at(i).id);
	units.at(i).removed = true;
	//units.erase(units.begin() + i);
}

sf::Vector2i LimitToBoard(sf::Vector2f tile, Board* board) {
	return tile.x < board->boardSize.x&&tile.x >= 0 && tile.y < board->boardSize.y&&tile.y >= 0 ? sf::Vector2i(tile) : sf::Vector2i(-1, -1);
}

void Scene::MouseHover(sf::Vector2i pos)
{
	sf::Vector2i old = mouseTile;
	mouseTile = LimitToBoard(sf::Vector2f(board.boardSize.x*pos.x/tiles.at(0).getLocalBounds().width, board.boardSize.y*pos.y / tiles.at(0).getLocalBounds().height), &board);
	if (old != mouseTile) {
		boardUi.at(0).setColor(sf::Color(255, 255, 255, mouseTile.x < 0 ? 0 : 255));
		boardUi.at(0).setPosition(mouseTile.x*tiles.at(0).getLocalBounds().width / board.boardSize.x, mouseTile.y*tiles.at(0).getLocalBounds().height / board.boardSize.y);
		texts.at(5).setString(board.GetTile(mouseTile.x, mouseTile.y).Print());
		Unit* u = Unit::GetUnit(board.GetTile(mouseTile.x,mouseTile.y).unit);
		texts.at(6).setString(u != nullptr ? u->Print() : "");
		if (currentUnit >= 0) {
			std::vector<sf::Vector2i> path = board.FindPath(FindUnit(currentUnit)->tile, mouseTile);
			boardUiV.clear();
			for (unsigned int i = 0; i < path.size(); i++) {
				boardUiV.append(sf::Vertex(sf::Vector2f(path.at(i).x*Constants::tileSize+Constants::tileSize/2, path.at(i).y*Constants::tileSize+Constants::tileSize/2), sf::Color(255, 50, 50, 255)));
			}
		}
	}
}

Unit * Scene::FindUnit(int id)
{
	if (id != -1) {
		for (int i = 0; i < units.size(); i++) {
			if (units.at(i).id == id) {
				return &units.at(i);
			}
		}
	}
	return nullptr;
}

void Scene::SetUnit(int id)
{
	Unit* u = FindUnit(id);
	if (u!=nullptr && FindUnit(id)->player == currentPlayer && currentPlayer>0) {
		currentUnit = id;
		update = true;
	}
}

void Scene::SetAction(std::string action)
{
	if (currentUnit == -1)
		currentAction = "";
	else
		currentAction = action;
	update = true;
}

void Scene::EndTurn()
{
	for (int i = 0; i < units.size(); i++) {
		units.at(i).EndOfTurn();
	}
	currentPlayer = NextFromList(currentPlayer, players);
	Unit* u = FindUnit(currentUnit);
	if (u == nullptr || (u->player != currentPlayer && currentPlayer != 0)) {
		currentUnit = -1;
		currentAction = "";
	}
	update = true;
}

void Scene::UpdateState()
{
	update = false;
	int index = -1;
	for (int i = 0; i < players.size(); i++) {
		if (players.at(i) == currentPlayer) {
			index = i;
		}
	}
	players.clear();
	for (int i = 0; i < units.size(); i++) {
		if (units.at(i).Dead()) {
			if(!units.at(i).removed)
				RemoveUnit(i);
		}
		else if (std::find(players.begin(), players.end(), units.at(i).player) == players.end()) {
			players.push_back(units.at(i).player);
		}
	}
	currentPlayer = players.at(index < players.size() ? index : index - 1);
	Tile t = board.GetTile(mouseTile.x, mouseTile.y);
	texts.at(0).setString("Current player: " + std::to_string(currentPlayer));
	texts.at(1).setString(currentUnit != -1 ? FindUnit(currentUnit)->Print(true) : "");
	texts.at(2).setString(currentUnit != -1 ? "Action: " + currentAction : "");
	texts.at(5).setString(t.Print());
	Unit* u = Unit::GetUnit(t.unit);
	texts.at(6).setString(u != nullptr ? u->Print() : "");
	for (int i = 0; i < 9; i++) {
		if (i == 4)
			continue;
		texts.at(i + 1).setPosition(texts.at(i+1).getPosition().x, texts.at(i).getGlobalBounds().top + texts.at(i).getGlobalBounds().height + 20);
	}
}

void Scene::Click()
{
	if (menu.state<0 && mouseTile.x < 0) {
		currentUnit = -1;
		currentAction = "";
		update = true;
		return;
	}

	Unit* u = FindUnit(currentUnit);
	bool validUnit = u != nullptr && u->player == currentPlayer;

	if (menu.state>=0 && menu.player>=0) {
		bool update = false;
		u = FindUnit(menu.player);
		validUnit = u != nullptr;
		if (validUnit) {
			for (unsigned int i = 0; i < menu.attributes.size(); i++) {
				if (menu.attributes.at(i).sprite.getGlobalBounds().contains(mousePos - menu.GetOffset())) {
					if (menu.attributes.at(i).value == 1) {
						if (u->LevelUp(menu.attributes.at(i).name)) {
							Messages::Add(u->Print(false, true) + " leveled up!");
							update = true;
						}
						else {
							Messages::Notice("Not enough XP");
						}
					}
				}
			}
			for (unsigned int i = 0; i < menu.items.size(); i++) {
				if (menu.items.at(i).sprite.getGlobalBounds().contains(mousePos - menu.GetOffset())) {
					if (menu.items.at(i).name == "weapon" && menu.items.at(i).value>=0) {
						if (u->SwitchWeapon(menu.items.at(i).value)) {
							update = true;
							if (!combat)
								EndTurn();
						}
					}
					if (menu.items.at(i).name == "equipment" && menu.items.at(i).value >= 0) {
						if (u->Equip(menu.items.at(i).value)) {
							update = true;
							if (!combat)
								EndTurn();
						}
					}
					if (menu.items.at(i).name == "item" && menu.items.at(i).value >= 0) {
						currentAction = "item";
						u->SwitchItem(menu.items.at(i).value);
						menu.Construct(currentUnit);
					}
				}
			}
		}
		if (update) {
			update = false;
			menu.Refresh();
		}
		return;
	}
	
	if(currentAction!="attack")
		SetUnit(board.GetTile(mouseTile.x, mouseTile.y).unit);
	
	if (currentUnit != -1 && !Messages::prompting && validUnit) {
		if (currentAction == "move") {
			if (u->MoveTo(mouseTile)) {
				if(!combat)
					EndTurn();
				board.refresh = true;
			}
		}
		else if (currentAction == "attack") {
			if (u->AttackTo(mouseTile) && !combat)
				EndTurn();
		}
		else if (currentAction == "loot") {
			if (!u->LootFrom(mouseTile))
				Messages::Notice("Nothing to loot there");
			else if (!combat)
				EndTurn();
		}
		else if (currentAction == "item") {
			if (!u->UseItem(mouseTile))
				Messages::Notice("Invalid item usage");
			else if (!combat)
				EndTurn();
		}
	}
	else {
		SetAction("");
	}
	update = true;
}

void Scene::KeyPress(sf::Keyboard::Key key)
{
	Unit* u = FindUnit(currentUnit);
	bool validUnit = u!=nullptr && u->player == currentPlayer;

	if (key == sf::Keyboard::E && currentPlayer!=0) {
		EndTurn();
	}
	if (!Messages::prompting && validUnit) {
		if (key == sf::Keyboard::M) {
			SetAction("move");
		}
		else if (key == sf::Keyboard::A) {
			SetAction("attack");
		}
		else if (key == sf::Keyboard::L) {
			SetAction("loot");
		}
		else if (key == sf::Keyboard::U) {
			SetAction("item");
		}
		else if (key == sf::Keyboard::C) {
			if(menu.Construct(currentUnit))
				menu.SetRect(sf::FloatRect(Constants::fixedView.width*0.2f, Constants::fixedView.height * 0.1f, Constants::fixedView.width*0.6f, Constants::fixedView.height*0.75f));
		}
		else if (key == sf::Keyboard::I) {
			u->SwitchItem();
		}
		else if (key == sf::Keyboard::W) {
			if (u->SwitchWeapon() && !combat)
				EndTurn();
		}
		else if (key == sf::Keyboard::Q) {
			u->GetWeapon()->SwitchAttack();
		}
		else {
			SetAction("");
		}
	}
	update = true;
}
