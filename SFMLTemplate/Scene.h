#pragma once
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Time.h"
#include "Board.h"

class Scene
{
public:
	Scene();
	~Scene();
	std::vector<sf::Sprite>* Update();
	void AddTile(sf::Sprite* spr);
	void SetTileSize(int size) { board.SetTileSize(size); };
	void MouseHover(sf::Vector2i pos);
	Board* GetBoard() { return &board; };
private:
	std::vector<sf::Sprite> tiles;
	Board board;
	sf::Vector2i mouseTile;
};

