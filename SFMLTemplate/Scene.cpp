#include "Scene.h"

Scene::Scene()
{
	for (int i = 0; i < Constants::boardSize; i++) {
		board.InsertTile(i, i, Tile('g'));
	}
	const sf::Texture& bg = board.GetTexture(true)->getTexture();
	tiles.push_back(sf::Sprite(bg));
}

Scene::~Scene()
{
}

std::vector<sf::Sprite>* Scene::Update()
{
	if (Resources::isReady()) {
		tiles.at(0) = sf::Sprite(board.GetTexture(board.refresh)->getTexture());
		board.refresh = false;
	}
	return &tiles;
}
