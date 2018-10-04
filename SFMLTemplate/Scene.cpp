#include "Scene.h"

Scene::Scene()
{
	for (int i = 0; i < Constants::boardSize; i++) {
		board.InsertTile(i, i, Tile('g'));
	}
	const sf::Texture& bg = board.GetTexture(true)->getTexture();
	tiles.push_back(sf::Sprite(bg));
	tiles.push_back(sf::Sprite(*Resources::GetTexture('o')));
}

Scene::~Scene()
{
}

std::vector<sf::Sprite>* Scene::Update()
{
	if (Resources::isReady()) {
		tiles.at(0).setTexture(board.GetTexture(board.refresh)->getTexture());
		tiles.at(1).setTexture(*Resources::GetTexture('o'));
		board.refresh = false;
	}
	return &tiles;
}

void Scene::AddTile(sf::Sprite * spr)
{
	tiles.push_back(*spr);
}
