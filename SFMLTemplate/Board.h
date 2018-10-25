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
	bool CheckLOS(int x1, int y1, int x2, int y2, bool visual=false);
	std::vector<sf::Vector2i> FindPath(sf::Vector2i from, sf::Vector2i to);
	void SetTileSize(float size);
	float GetTileSize();
	Tile GetTile(unsigned short x, unsigned short y);
	sf::Vector2i boardSize;
	bool refresh;
	sf::RenderTexture* GetTexture(bool refresh = false);
	sf::VertexArray debug;
private:
	void MakeRoom(short x1, short x2, short y1, short y2, short chance);
	float tileSize;
	Tile tiles[Constants::boardSize][Constants::boardSize];
	static sf::RenderTexture rTex;
};
