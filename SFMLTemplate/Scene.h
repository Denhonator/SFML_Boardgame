#pragma once
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Time.h"
#include "Board.h"
#include "Unit.h"

class Scene
{
public:
	Scene();
	~Scene();
	std::vector<sf::Sprite>* Update();
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
	std::vector<sf::Sprite> ui;
	std::vector<sf::Sprite> tiles;
	std::vector<Unit> units;
	std::vector<sf::Text> texts;
	std::vector<unsigned int> drawTiles;
	std::vector<unsigned int> drawUnits;
private:
	std::thread AI;
	std::vector<int> AIInCombat;
	void CheckAICombat();
	void AITurn();
	int AIFindTarget();
	int NextFromList(int to, std::vector<int> list);
	void SetCombat(bool val);
	bool AIReady = false;
	bool combat = false;
	bool update;
	int aiUnit = -1;
	Board board;
	sf::Vector2i mouseTile;
	std::vector<int> players;
	int currentPlayer;
	int currentUnit;
	std::string currentAction;
};
