#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "Scene.h"
#include "Time.h"
#include <stdio.h>

class Main
{
public:
	Main();
	~Main();
	 void Events();
	 void Update();
	 void FixedUpdate();
	 void Draw();
	 sf::RenderWindow* GetWindow();
private:
	 sf::RenderWindow window;
	 std::vector<sf::Sprite>* tiles;
	 std::vector<sf::Sprite>* sprites;
	 Scene mainScene;
	 Time timeObject;
	 sf::View view;
	 sf::Vector2i mousePos;
	 sf::Vector2i mouseTile;
};

