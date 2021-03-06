#include "Board.h"

sf::RenderTexture Board::rTex;

Board::Board()
{
	boardSize = sf::Vector2i(30,30);
	tileSize = 30;
	refresh = true;
	rTex.create(2048, 2048);
	for (unsigned short i = 0; i < boardSize.x; i++) {
		for (unsigned short j = 0; j < boardSize.y; j++) {
			tiles[i][j] = Tile();
		}
	}
}

Board::~Board()
{
}

void Board::InsertTile(unsigned short x, unsigned short y, Tile tile)
{
	tiles[x][y] = tile;
}

void Board::SetTileSize(float size)
{
	if(size!=tileSize)
		refresh = true;
	tileSize = size;
}

sf::RenderTexture * Board::GetTexture(bool refresh)
{
	if (refresh) {
		rTex.clear();
		rTex.setView(sf::View(sf::FloatRect(0, 0, boardSize.x*tileSize, boardSize.y*tileSize)));
		for (unsigned short i = 0; i < boardSize.x; i++) {
			for (unsigned short j = 0; j < boardSize.y; j++) {
				char s = tiles[i][j].sprite;
				sf::Sprite t;
				t.setTexture(*Resources::GetTexture(s));
				t.setPosition(i*tileSize, j*tileSize);
				t.setScale(sf::Vector2f(tileSize/t.getLocalBounds().width, tileSize/t.getLocalBounds().height));
				rTex.draw(t);
			}
		}
		rTex.display();
	}
	return &rTex;
}
