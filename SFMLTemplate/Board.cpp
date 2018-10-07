#include "Board.h"

sf::RenderTexture Board::rTex;

Board::Board()
{
	tileSize = 30;
	boardSize = sf::Vector2i(30,30);
	refresh = true;
	rTex.create(2048, 2048);
	for (unsigned short i = 0; i < boardSize.x; i++) {
		for (unsigned short j = 0; j < boardSize.y; j++) {
			tiles[i][j] = Tile();
		}
	}
	Constants::tileSize = GetTileSize();
}

Board::~Board()
{
}

void Board::InsertTile(unsigned short x, unsigned short y, Tile tile)
{
	tiles[x][y] = tile;
	Tile::tileRef[x] = &tiles[x][0];
}

void Board::SetTileSize(float size)
{
	if(size!=tileSize)
		refresh = true;
	tileSize = size;
}

float Board::GetTileSize()
{
	return rTex.getSize().x / boardSize.x;
}

Tile Board::GetTile(unsigned short x, unsigned short y)
{
	if (x < boardSize.x&&y < boardSize.y)
		return tiles[x][y];
	return Tile();
}

sf::RenderTexture * Board::GetTexture(bool refresh)
{
	if (refresh) {
		rTex.clear();
		rTex.setView(sf::View(sf::FloatRect(0, 0, boardSize.x*tileSize, boardSize.y*tileSize)));
		for (unsigned short i = 0; i < boardSize.x; i++) {
			for (unsigned short j = 0; j < boardSize.y; j++) {
				sf::Sprite t;
				t.setTexture(*Resources::GetTexture("tiles/"+tiles[i][j].sprite+"/"+ tiles[i][j].sprite));
				t.setPosition(i*tileSize, j*tileSize);
				t.setScale(sf::Vector2f(tileSize/t.getLocalBounds().width, tileSize/t.getLocalBounds().height));
				rTex.draw(t);
			}
		}
		rTex.display();
	}
	return &rTex;
}
