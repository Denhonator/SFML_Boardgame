#include "Board.h"

sf::RenderTexture Board::rTex;

Board::Board()
{
	srand(time(NULL));
	tileSize = 30;
	boardSize = sf::Vector2i(30,30);
	refresh = false;
	rTex.create(2048, 2048);
	for (unsigned int i = 0; i < boardSize.x; i++) {
		for (unsigned int j = 0; j < boardSize.y; j++) {
			tiles[i][j] = Tile();
		}
	}
	Constants::tileSize = GetTileSize();
	debug.setPrimitiveType(sf::Lines);
}

Board::~Board()
{
}

void Board::InsertTile(unsigned short x, unsigned short y, Tile tile)
{
	tiles[x][y] = tile;
	Tile::tileRef[x] = &tiles[x][0];
}

void Board::MakeRoom(short x1, short x2, short y1, short y2, short chance)
{
	x2 = std::min((int)x2, boardSize.x);
	y2 = std::min((int)y2, boardSize.y);
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			if(rand()%101<chance)
				InsertTile(i, j, Tile("grass"));
		}
	}
}

void Board::Randomize()
{
	MakeRoom(0, 2, 0, 2, 100);	//Starting area

	MakeRoom(0, boardSize.x-1, 0, 0, 50);	//Edges
	MakeRoom(0, 0, 0, boardSize.y-1, 50);
	MakeRoom(0, boardSize.x - 1, boardSize.y-1, boardSize.y-1, 50);
	MakeRoom(boardSize.x-1, boardSize.x-1, 0, boardSize.y - 1, 50);

	MakeRoom(0, boardSize.x - 1, 0, boardSize.y - 1, 35);	//All around
	for (unsigned int i = 0; i < boardSize.x; i += 6) {
		for (unsigned int j = 0; j < boardSize.y; j += 6) {
			MakeRoom(i, i + 4, j, j + 4, 85);				//Rooms
		}
	}
}

bool Board::CheckLOS(int x1, int y1, int x2, int y2, bool visual)
{
	float off = 0.01f;
	std::vector<sf::Vector2f> testPoints = { sf::Vector2f(x1+off,y1+off), sf::Vector2f(x1+1-off,y1+off), sf::Vector2f(x1+off,y1+1-off), sf::Vector2f(x1+1-off,y1+1-off) };
	std::vector<sf::Vector2f> destPoints = { sf::Vector2f(x2 + off,y2 + off), sf::Vector2f(x2 + 1 - off,y2 + off), sf::Vector2f(x2 + off,y2 + 1 - off), sf::Vector2f(x2 + 1 - off,y2 + 1 - off) };
	sf::Vector2f dest;
	sf::Vector2f ray;
	sf::Vector2f dir;
	sf::Vector2f add;
	float speed = 0.1f;
	float length;
	int loops = (float)Constants::boardSize/speed;

	for (unsigned int i = 0; i < destPoints.size(); i++) {
		dest = destPoints.at(i);
		for (unsigned int k = 0; k < testPoints.size(); k++) {
			ray = testPoints.at(k);
			length = std::sqrt((dest.x - ray.x)*(dest.x - ray.x) + (dest.y - ray.y)*(dest.y - ray.y));
			dir = sf::Vector2f((dest.x - ray.x) / length, (dest.y - ray.y) / length);
			add = sf::Vector2f(dir.x*speed, dir.y*speed);

			loops = length / speed;

			for (unsigned int l = 0; l <= loops; l++) {
				if ((int)ray.x < 0 || (int)ray.x >= Constants::boardSize || (int)ray.y < 0 || (int)ray.y >= Constants::boardSize)
					break;
				if (visual)
					tiles[(int)ray.x][(int)ray.y].seen = true;
				if ((int)ray.x == (int)dest.x && (int)ray.y == (int)dest.y) {
					return true;
				}
				if (tiles[(int)ray.x][(int)ray.y].sprite == "0") {
					break;
				}
				ray += add;
			}
		}
	}
	return false;
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
		debug.clear();
		rTex.setView(sf::View(sf::FloatRect(0, 0, boardSize.x*tileSize, boardSize.y*tileSize)));
		for (unsigned int i = 0; i < boardSize.x; i++) {
			for (unsigned int j = 0; j < boardSize.y; j++) {
				sf::Sprite t;
				t.setTexture(*Resources::GetTexture("tiles/" + tiles[i][j].sprite + "/" + tiles[i][j].sprite));
				t.setPosition(i*tileSize, j*tileSize);
				t.setScale(sf::Vector2f(tileSize / t.getLocalBounds().width, tileSize / t.getLocalBounds().height));
				if (!tiles[i][j].seen)
					t.setColor(sf::Color(50, 50, 50, 255));
				rTex.draw(t);
			}
		}
		rTex.display();
	}
	return &rTex;
}
