#pragma once
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Time.h"

class Scene
{
public:
	Scene();
	~Scene();
	std::vector<sf::Sprite>* Update();
private:
	std::vector<sf::Sprite> tiles;
	sf::Texture grass;
};

