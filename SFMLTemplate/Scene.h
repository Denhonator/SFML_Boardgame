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
	void SetTileSize(int size) { board.SetTileSize(size); };
private:
	std::vector<sf::Sprite> tiles;
	Board board;
};

