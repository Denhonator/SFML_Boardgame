#pragma once
#include "Tile.h"
#include "Resources.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>
class Board
{
public:
	Board();
	~Board();
	void InsertTile(unsigned short x, unsigned short y, Tile tile);
	void SetTileSize(float size);
	float GetTileSize() { return tileSize; };
	sf::Vector2i boardSize;
	bool refresh;
	sf::RenderTexture* GetTexture(bool refresh = false);
private:
	float tileSize;
	Tile tiles[Constants::boardSize][Constants::boardSize];
	static sf::RenderTexture rTex;
};
