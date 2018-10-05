#include "Scene.h"

Scene::Scene()
{
	srand(time(NULL));
	drawTiles = { 0 };
	for (int i = 0; i < board.boardSize.x; i++) {
		for (int j = 0; j < board.boardSize.y; j++) {
			if(rand()%101>30)
				board.InsertTile(i, j, Tile("grass"));
		}
	}
	const sf::Texture& bg = board.GetTexture(true)->getTexture();
	tiles.push_back(sf::Sprite(bg));
	ui.push_back(sf::Sprite(*Resources::GetTexture("ui/outline")));
	ui.at(0).setScale(Constants::tileSize / ui.at(0).getLocalBounds().width, Constants::tileSize / ui.at(0).getLocalBounds().height);
	mouseTile = sf::Vector2i(0, 0);
	AddUnit(&Unit("swordguy", 1));
	currentPlayer = 1;
}

Scene::~Scene()
{
}

std::vector<sf::Sprite>* Scene::Update()
{
	if (Resources::isReady()) {
		tiles.at(0).setTexture(board.GetTexture(board.refresh)->getTexture());
		board.refresh = false;
	}
	return &tiles;
}

void Scene::AddTile(sf::Sprite * spr)
{
	tiles.push_back(*spr);
	drawTiles.push_back(tiles.size() - 1);
}

void Scene::AddUnit(Unit * unit)
{
	units.push_back(*unit);
	drawUnits.push_back(tiles.size() - 1);
}

sf::Vector2i LimitToBoard(sf::Vector2f tile, Board* board) {
	return tile.x < board->boardSize.x&&tile.x >= 0 && tile.y < board->boardSize.y&&tile.y >= 0 ? sf::Vector2i(tile) : sf::Vector2i(-1, -1);
}

void Scene::MouseHover(sf::Vector2i pos)
{
	mouseTile = LimitToBoard(sf::Vector2f(board.boardSize.x*pos.x/tiles.at(0).getLocalBounds().width, board.boardSize.y*pos.y / tiles.at(0).getLocalBounds().height), &board);
	ui.at(0).setColor(sf::Color(255, 255, 255, mouseTile.x < 0 ? 0 : 255));
	ui.at(0).setPosition(mouseTile.x*tiles.at(0).getLocalBounds().width / board.boardSize.x, mouseTile.y*tiles.at(0).getLocalBounds().height / board.boardSize.y);
	//printf("%d, %d\n", mouseTile.x, mouseTile.y);
}

void Scene::Click()
{
	for (int i = 0; i < units.size(); i++) {
		if (units.at(i).player == currentPlayer) {
			units.at(i).MoveTo(mouseTile, &board.GetTile(mouseTile.x, mouseTile.y));
		}
	}
}
