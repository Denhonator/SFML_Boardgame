#include "Scene.h"

Scene::Scene()
{
	if (!grass.loadFromFile("grass.png"))
		printf("FAILED TO LOAD IMAGE");
	tiles.push_back(sf::Sprite(grass));
	tiles.at(0).setPosition(0, 0);
}

Scene::~Scene()
{
}

std::vector<sf::Sprite>* Scene::Update()
{
	tiles.at(0).move(Time::Mult(), 0);
	return &tiles;
}
