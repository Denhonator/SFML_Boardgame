#include "Scene.h"

Scene::Scene()
{
	srand(time(NULL));
	board.Randomize();
	for (short i = 0; i < board.boardSize.x; i++) {
		for (short j = 0; j < board.boardSize.y; j++) {
			if (i + j > 6 && board.GetTile(i, j).sprite == "grass"&&board.GetTile(i, j).unit == -1 && rand() % 101 > 95)
				AddUnit(&Unit("cave crab", 0, sf::Vector2i(i, j)));
		}
	}
	const sf::Texture& bg = board.GetTexture(true)->getTexture();
	AddTile(&sf::Sprite(bg));
	ui.push_back(sf::Sprite(*Resources::GetTexture("ui/outline")));
	ui.at(0).setScale(Constants::tileSize / ui.at(0).getLocalBounds().width, Constants::tileSize / ui.at(0).getLocalBounds().height);
	for (int i = 0; i < 10; i++) {
		texts.push_back(sf::Text("", *Resources::GetFont("default.ttf")));
		texts.at(i).setPosition(sf::Vector2f(i>4 ? 2100 : 0, i%5 * 200));
	}
	mouseTile = sf::Vector2i(0, 0);
	AddUnit(&Unit("swordguy", 1, mouseTile,"denho"));
	currentPlayer = 1;
	currentUnit = -1;
	currentAction = "";
	players = { 1 };
	UpdateState();
}

Scene::~Scene()
{
	if (AI.joinable())
		AI.join();
}

void Scene::CheckAICombat()
{
	AIInCombat.clear();
	for (aiUnit = 0; aiUnit < units.size(); aiUnit++) {
		if (!units.at(aiUnit).Dead() && units.at(aiUnit).player == 0) {
			if(AIFindTarget()>=0)
				AIInCombat.push_back(aiUnit);
		}
	}
	aiUnit = -1;
}

void Scene::AITurn()
{
	short temp = aiUnit;
	for (short i = aiUnit + 1; i < units.size(); i++) {
		if (units.at(i).player == 0 && !units.at(i).Dead()) {
			aiUnit = i;
			break;
		}
	}
	if (temp == aiUnit) {
		aiUnit = -1;
		EndTurn();
	}
	else {
		short index = AIFindTarget();
		if (index == -1) {
			AIReady = true;
			return;
		}
		short dist = Unit::Distance(units.at(aiUnit).tile, units.at(index).tile);

		if (index != -1 && dist < 8 && dist > 1) {
			for (short i = 0; i < 5; i++) {
				if (units.at(aiUnit).MoveTowards(units.at(index).tile))
					sf::sleep(sf::milliseconds(300));
				if (Unit::Distance(units.at(aiUnit).tile, units.at(index).tile) < 2) {
					break;
				}
			}
		}

		if (Unit::Distance(units.at(aiUnit).tile, units.at(index).tile) < 2) {
			while (units.at(aiUnit).AttackTo(units.at(index).tile)) {
				sf::sleep(sf::milliseconds(300));
				if (units.at(index).Dead()) {
					index = AIFindTarget();
					if (index == -1) {
						break;
					}
				}
			}
		}
	}
	AIReady = true;
}

short Scene::AIFindTarget()
{
	short dist = 100;
	short index = -1;
	for (short i = 0; i < units.size(); i++) {
		if (units.at(i).player == 0)
			continue;
		short newDist = Unit::Distance(units.at(aiUnit).tile, units.at(i).tile);
		if (newDist < dist && !units.at(i).Dead()) {
			dist = newDist;
			index = i;
		}
	}
	if (dist >= 8)
		return -1;
	return index;
}

short Scene::NextFromList(short to, std::vector<short> list)
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

std::vector<sf::Sprite>* Scene::Update()
{
	if (currentPlayer == 0 && players.size()>1) {
		if (aiUnit == -1) {
			short prev = AIInCombat.size();
			CheckAICombat();
			if (AIInCombat.size()) {
				SetCombat(true);
				if (prev) {
					AIReady = false;
					AI = std::thread(&Scene::AITurn, this);
				}
				else
					currentPlayer = NextFromList(currentPlayer, players);
			}
			else {
				SetCombat(false);
				currentPlayer = NextFromList(currentPlayer, players);
			}
		}
		if (AIReady&&currentPlayer==0) {
			AI.join();
			AIReady = false;
			AI = std::thread(&Scene::AITurn, this);
		}
	}
	else if (AIReady) {
		AI.join();
		AIReady = false;
	}
	if (board.refresh) {
		tiles.at(0).setTexture(board.GetTexture(board.refresh)->getTexture());
		board.refresh = false;
	}
	if (update)
		UpdateState();
	return &tiles;
}

void Scene::AddTile(sf::Sprite * spr)
{
	tiles.push_back(*spr);
	drawTiles.push_back(tiles.size() - 1);
}

void Scene::AddUnit(Unit * unit)
{
	units.push_back(*unit);
	//drawUnits.push_back(units.size() - 1);
	bool found = false;
	for (int i = 0; i < players.size(); i++) {
		if (unit->player == players.at(i))
			found = true;
	}
	if (!found)
		players.push_back(unit->player);
	Unit::unit = &units;
}

void Scene::RemoveUnit(unsigned short i)
{
	if (currentUnit == units.at(i).id) {
		currentUnit = -1;
		currentAction = "";
	}
	Tile::tileRef[units.at(i).tile.x][units.at(i).tile.y].unit = -1;
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
		ui.at(0).setColor(sf::Color(255, 255, 255, mouseTile.x < 0 ? 0 : 255));
		ui.at(0).setPosition(mouseTile.x*tiles.at(0).getLocalBounds().width / board.boardSize.x, mouseTile.y*tiles.at(0).getLocalBounds().height / board.boardSize.y);
		texts.at(5).setString(board.GetTile(mouseTile.x, mouseTile.y).Print());
		Unit* u = Unit::GetUnit(board.GetTile(mouseTile.x,mouseTile.y).unit);
		texts.at(6).setString(u != nullptr ? u->Print() : "");
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
	if (u!=nullptr && FindUnit(id)->player == currentPlayer) {
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
	short index = -1;
	for (short i = 0; i < players.size(); i++) {
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
	if (mouseTile.x < 0) {
		currentUnit = -1;
		currentAction = "";
		update = true;
		return;
	}
	if(currentAction!="attack")
		SetUnit(board.GetTile(mouseTile.x, mouseTile.y).unit);

	Unit* u = FindUnit(currentUnit);
	bool validUnit = u!=nullptr && u->player == currentPlayer;
	
	if (currentUnit != -1 && !Messages::prompting && validUnit) {
		if (currentAction == "move") {
			if (u->MoveTo(mouseTile)&&!combat)
				EndTurn();
		}
		else if (currentAction == "attack") {
			if (u->AttackTo(mouseTile) && !combat)
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
	if (currentUnit != -1 && !Messages::prompting && validUnit) {
		if (key == sf::Keyboard::M) {
			SetAction("move");
		}
		else if (key == sf::Keyboard::A) {
			SetAction("attack");
		}
		else if (key == sf::Keyboard::W) {
			u->SwitchWeapon();
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
