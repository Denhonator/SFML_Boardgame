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
	debug.setPrimitiveType(sf::Quads);
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

std::vector<sf::Vector2i> Board::FindPath(sf::Vector2i from, sf::Vector2i to)	//A* algorithm
{
	if (to.x<0 || to.y<0 || to.x>Constants::boardSize || to.y>Constants::boardSize || tiles[to.x][to.y].sprite == "0" || tiles[to.x][to.y].unit>=0)	//Instantly return empty vector if input is invalid
		return std::vector<sf::Vector2i>();
	std::vector<sf::Vector2i> finalPath;
	std::vector<sf::Vector2i> open;
	std::vector<sf::Vector2i> closed = { from };
	std::vector<sf::Vector2i> voffs = Resources::Voffs();
	std::map<int, int> costs = { {from.x*Constants::boardSize+from.y, 0} };
	std::map<int, sf::Vector2i> direction;
	sf::Vector2i temp;
	int closedIndex = 0;
	int openIndex = 0;
	float dist = 999;
	float tempDist = 999;
	bool found = false;

	do {
		for (unsigned int i = 0; i < voffs.size(); i++) {	//Add to open from closed at index
			if (voffs.at(i).x||voffs.at(i).y) {
				temp = closed.at(closedIndex) + voffs.at(i);
				if (std::find(closed.begin(), closed.end(), temp) != closed.end() || std::find(open.begin(), open.end(), temp) != open.end()	//Don't add what's in a list already
					|| temp.x < 0 || temp.y < 0 || temp.x >= Constants::boardSize || temp.y >= Constants::boardSize								//Only valid and walkable squares
					|| tiles[temp.x][temp.y].sprite == "0" || tiles[temp.x][temp.y].unit >= 0
					|| (tiles[closed.at(closedIndex).x + voffs.at(i).x][closed.at(closedIndex).y].sprite == "0"									//Don't cross diagonally between two walls
						&& tiles[closed.at(closedIndex).x][closed.at(closedIndex).y + voffs.at(i).y].sprite == "0"))
					continue;

				open.push_back(temp);
				direction[temp.x*Constants::boardSize + temp.y] = closed.at(closedIndex);
				int moveCost = 3;
				if (std::abs(voffs.at(i).x) + std::abs(voffs.at(i).y) == 2)
					moveCost *= 3;	//Weight for diagonal movement
				costs[temp.x*Constants::boardSize+temp.y] = costs[closed.at(closedIndex).x*Constants::boardSize+closed.at(closedIndex).y] + moveCost;
			}
		}
		dist = 999;
		tempDist = 999;
		for (unsigned int i = 0; i < open.size(); i++) {	//Pick estimated closest and add to closed list
			if (open.at(i) == to) {
				openIndex = i;
				found = true;
				break;
			}
			temp = sf::Vector2i(to.x - open.at(i).x, to.y - open.at(i).y);
			//tempDist = std::sqrt(temp.x*temp.x + temp.y*temp.y);
			tempDist = std::abs(temp.x)*2 + std::abs(temp.y)*2 + costs[open.at(i).x*Constants::boardSize+open.at(i).y];
			if (tempDist < dist) {
				dist = tempDist;
				openIndex = i;
			}
		}
		if (open.size()) {
			closed.push_back(open.at(openIndex));
			open.erase(open.begin() + openIndex);
			closedIndex++;
		}
	} while (open.size()&&!found);
	if (found) {
		debug.clear();
		for (unsigned int i = 0; i < closed.size(); i++) {
			debug.append(sf::Vertex(sf::Vector2f(closed.at(i).x*Constants::tileSize, closed.at(i).y*Constants::tileSize), sf::Color(25, 25, 255, 25)));
			debug.append(sf::Vertex(sf::Vector2f(closed.at(i).x*Constants::tileSize+Constants::tileSize, closed.at(i).y*Constants::tileSize), sf::Color(25, 25, 255, 25)));
			debug.append(sf::Vertex(sf::Vector2f(closed.at(i).x*Constants::tileSize+Constants::tileSize, closed.at(i).y*Constants::tileSize+Constants::tileSize), sf::Color(25, 25, 255, 25)));
			debug.append(sf::Vertex(sf::Vector2f(closed.at(i).x*Constants::tileSize, closed.at(i).y*Constants::tileSize+Constants::tileSize), sf::Color(25, 25, 255, 25)));
		}
		found = false;
		closedIndex = closed.size() - 1;

		finalPath.push_back(closed.at(closedIndex));
		closed.erase(closed.begin() + closedIndex, closed.end());
		while (finalPath.at(finalPath.size() - 1) != from) {
			finalPath.push_back(direction[finalPath.at(finalPath.size() - 1).x*Constants::boardSize + finalPath.at(finalPath.size() - 1).y]);
		}
	}
	if (finalPath.size())
		std::reverse(finalPath.begin(), finalPath.end());
	return finalPath;
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
