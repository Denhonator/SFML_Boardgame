#include "Scene.h"

Scene::Scene()
{
	for (int i = 0; i < Constants::boardSize; i++) {
		board.InsertTile(i, i, Tile('g'));
	}
	const sf::Texture& bg = board.GetTexture(true)->getTexture();
	tiles.push_back(sf::Sprite(bg));
	tiles.push_back(sf::Sprite(*Resources::GetTexture('o')));
	tiles.at(1).setScale(tiles.at(0).getLocalBounds().width/board.boardSize.x / tiles.at(1).getLocalBounds().width, tiles.at(0).getLocalBounds().height / board.boardSize.y / tiles.at(1).getLocalBounds().height);
	mouseTile = sf::Vector2i(0, 0);
}

Scene::~Scene()
{
}

std::vector<sf::Sprite>* Scene::Update()
{
	if (Resources::isReady()) {
		tiles.at(0).setTexture(board.GetTexture(board.refresh)->getTexture());
		if(board.refresh)
			tiles.at(1).setTexture(*Resources::GetTexture('o'));
		board.refresh = false;
	}
	return &tiles;
}

void Scene::AddTile(sf::Sprite * spr)
{
	tiles.push_back(*spr);
}

sf::Vector2i LimitToBoard(sf::Vector2f tile, Board* board) {
	return tile.x < board->boardSize.x&&tile.x >= 0 && tile.y < board->boardSize.y&&tile.y >= 0 ? sf::Vector2i(tile) : sf::Vector2i(-1, -1);
}

void Scene::MouseHover(sf::Vector2i pos)
{
	mouseTile = LimitToBoard(sf::Vector2f(board.boardSize.x*pos.x/tiles.at(0).getLocalBounds().width, board.boardSize.y*pos.y / tiles.at(0).getLocalBounds().height), &board);
	tiles.at(1).setColor(sf::Color(255, 255, 255, mouseTile.x < 0 ? 0 : 255));
	tiles.at(1).setPosition(mouseTile.x*tiles.at(0).getLocalBounds().width / board.boardSize.x, mouseTile.y*tiles.at(0).getLocalBounds().height / board.boardSize.y);
	//printf("%d, %d\n", mouseTile.x, mouseTile.y);
}
