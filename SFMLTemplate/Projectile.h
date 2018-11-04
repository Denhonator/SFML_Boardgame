#pragma once
#include "Resources.h"
#include "Time.h"
class Projectile
{
public:
	Projectile(sf::Vector2i from, sf::Vector2i to, std::string attackName, int effect = 0);
	~Projectile();
	void Update();
	sf::Sprite sprite;
	sf::Vector2f speed;
	float life;
	int effect;
};

