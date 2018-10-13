#pragma once
#include "Tile.h"
#include "Resources.h"
#include "Constants.h"

class Board
{
public:
	Board();
	~Board();
	void InsertTile(unsigned short x, unsigned short y, Tile tile);
	void Randomize();
	void SetTileSize(float size);
	float GetTileSize();
	Tile GetTile(unsigned short x, unsigned short y);
	sf::Vector2i boardSize;
	bool refresh;
	sf::RenderTexture* GetTexture(bool refresh = false);
private:
	void MakeRoom(short x1, short x2, short y1, short y2, short chance);
	float tileSize;
	Tile tiles[Constants::boardSize][Constants::boardSize];
	static sf::RenderTexture rTex;
};
