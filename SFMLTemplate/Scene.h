#pragma once
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Time.h"
#include "Board.h"
#include "Unit.h"
#include <random>

class Scene
{
public:
	Scene();
	~Scene();
	std::vector<sf::Sprite>* Update();
	void AddTile(sf::Sprite* spr);
	void AddUnit(Unit* unit);
	void SetTileSize(int size) { board.SetTileSize(size); };
	void MouseHover(sf::Vector2i pos);
	void Click();
	void KeyPress(sf::Keyboard::Key key);
	void SetUnit(int id);
	void SetAction(std::string action);
	Board* GetBoard() { return &board; };
	std::vector<sf::Sprite> ui;
	std::vector<sf::Sprite> tiles;
	std::vector<Unit> units;
	std::vector<sf::Text> texts;
	std::vector<unsigned int> drawTiles;
	std::vector<unsigned int> drawUnits;
private:
	Board board;
	sf::Vector2i mouseTile;
	int currentPlayer;
	Unit* currentUnit;
	std::string currentAction;
};
