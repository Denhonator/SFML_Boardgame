#pragma once
#include "Time.h"
#include "Board.h"
#include "Unit.h"
#include "Menu.h"

class Scene
{
public:
	Scene();
	~Scene();
	void Update();
	void FixedUpdate();
	void AddTile(sf::Sprite* spr);
	void AddUnit(Unit* unit);
	void RemoveUnit(unsigned int i);
	void SetTileSize(int size) { board.SetTileSize(size); };
	void MouseHover(sf::Vector2i pos);
	void Click();
	void KeyPress(sf::Keyboard::Key key);
	Unit* FindUnit(int id);
	void SetUnit(int id);
	void SetAction(std::string action);
	void EndTurn();
	void UpdateState();
	Board* GetBoard() { return &board; };
	sf::Vector2i GetMouseTile() { return mouseTile; };
	void SetMousePos(sf::Vector2f p) { mousePos = p; };
	sf::VertexArray boardUiV;
	std::vector<sf::Sprite> boardUi;
	std::vector<sf::Sprite> tiles;
	std::vector<Unit> units;
	std::vector<sf::Text> texts;
	std::vector<unsigned int> playerUnits;
	Menu menu;
private:
	bool running = true;
	std::thread AI;
	std::vector<int> AIInCombat;
	void CheckAICombat();
	void AITurn();
	int AIFindTarget(int unitIndex);
	int NextFromList(int to, std::vector<int> list);
	void SetCombat(bool val);
	bool combat = false;
	bool update;
	int aiUnit;
	Board board;
	sf::Vector2i mouseTile;
	sf::Vector2f mousePos;
	std::vector<int> players;
	int currentPlayer;
	int currentUnit;
	std::string currentAction;
};
